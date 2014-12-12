#include "LeapMotionPrivatePCH.h"

class PrivateLeapImage
{
public:
	~PrivateLeapImage()
	{
		if (!cleanupCalled)
		Cleanup();
	}
	void Cleanup()
	{
		//Ensures our rooted objects are unrooted so they can be GC'd
		if (imagePointer)
			imagePointer->RemoveFromRoot();
		if (imageR8Pointer)
			imageR8Pointer->RemoveFromRoot();
		if (distortionPointer)
			distortionPointer->RemoveFromRoot();
		cleanupCalled = true;
	}
	bool cleanupCalled = false;
	Leap::Image leapImage;

	UTexture2D* imagePointer = NULL;
	UTexture2D* imageR8Pointer = NULL;
	UTexture2D* distortionPointer = NULL;

	FVector FindBlob(const int32 SrcWidth, const int32 SrcHeight, int32 Start, std::vector<uint8>& chechPixel, int32 Stack, uint8* imageBuffer, int* stackLimit, const uint8 brightnessThresholdIn, const uint8 brightnessThresholdOut);
	TArray<FVector> TrackFromLeapImage(const int32 SrcWidth, const int32 SrcHeight, uint8* imageBuffer, const uint8 brightnessThresholdIn, const uint8 brightnessThresholdOut);

	float ClosestTimeOfApproach(FVector pos1, FVector vel1, FVector pos2, FVector vel2);
	float ClosestDistOfApproach(FVector pos1, FVector vel1, FVector pos2, FVector vel2);
	FVector ClosestPointOfApproach(FVector pos1, FVector vel1, FVector pos2, FVector vel2);

	UTexture2D* validImagePointer(UTexture2D* pointer, int32 pWidth, int32 pHeight, EPixelFormat format);

	UTexture2D* Texture8FromLeapImage(int32 SrcWidth, int32 SrcHeight, uint8* imageBuffer);
	UTexture2D* Texture32FromLeapImage(int32 SrcWidth, int32 SrcHeight, uint8* imageBuffer);
	UTexture2D* Texture32FromLeapDistortion(int32 SrcWidth, int32 SrcHeight, float* imageBuffer);
	UTexture2D* TextureFFromLeapDistortion(int32 SrcWidth, int32 SrcHeight, float* imageBuffer);

	int32 pixelsFound, pixelLimit;

	int32 invalidSizesReported = 0;
	bool ignoreTwoInvalidSizesDone = false;
	ULeapImage* self;
};

ULeapImage::ULeapImage(const FObjectInitializer &init) : UObject(init), _private(new PrivateLeapImage())
{
	_private->self = this;
}

ULeapImage::~ULeapImage()
{
	delete _private;
}

void ULeapImage::CleanupRootReferences()
{
	_private->Cleanup();
	this->RemoveFromRoot();
}

FVector PrivateLeapImage::FindBlob(const int32 SrcWidth, const int32 SrcHeight, int32 Start, std::vector<uint8>& checkPixel, int32 Stack, uint8* imageBuffer, int* stackLimit, const uint8 brightnessThresholdIn, const uint8 brightnessThresholdOut)
{
	float weight, sum;
	FVector pos, nPos;
	uint8* SrcPtr = NULL;
	uint8 Value;
	if (pixelsFound > pixelLimit) *stackLimit = 2;
	if (Stack >= 100) *stackLimit = 0;
	if (*stackLimit == 1) {

		Leap::Vector Dir = leapImage.rectify(Leap::Vector(floor(Start%SrcWidth), floor(Start / SrcWidth), 0.0f));
		//		Leap::Vector Dir = Leap::Vector(floor(Start%SrcWidth), floor(Start / SrcWidth), 0.0f);

		SrcPtr = (&imageBuffer[Start]);
		Value = *SrcPtr;
		weight = pow((float)(Value - brightnessThresholdOut), 2) + 1.f;
		//		weight = (float)(Value - brightnessThresholdOut) + 0.01f;
		pos = FVector(Dir.x * weight, Dir.y * weight, 0.0f);
		sum = weight;
		checkPixel[Start] = 1;
		pixelsFound++;
		//		if (Stack >= 15) *stackLimit = 0;
		//		ASSIMILATE THE SURROUNDING PIXELS
		//		if (Stack < 15) {//PREVENTS STACK OVERFLOW

		SrcPtr = (&imageBuffer[Start - 1]);
		Value = *SrcPtr;
		if ((floor(Start % SrcWidth) != 0) && (Value > brightnessThresholdOut) && (*stackLimit == 1)) if (checkPixel[Start - 1] == 0) {
			nPos = FindBlob(SrcWidth, SrcHeight, Start - 1, checkPixel, Stack + 1, imageBuffer, stackLimit, brightnessThresholdIn, brightnessThresholdOut);
			pos = FVector(pos.X + nPos.X, pos.Y + nPos.Y, 0.0f);
			if (nPos == FVector(0.0f, 0.0f, 0.0f)) *stackLimit = 0;
			sum += nPos.Z;
		}
		SrcPtr = (&imageBuffer[Start + 1]);
		Value = *SrcPtr;
		if ((floor(Start % SrcWidth) != SrcWidth - 1) && (Value > brightnessThresholdOut) && (*stackLimit == 1)) if (checkPixel[Start + 1] == 0) {
			nPos = FindBlob(SrcWidth, SrcHeight, Start + 1, checkPixel, Stack + 1, imageBuffer, stackLimit, brightnessThresholdIn, brightnessThresholdOut);
			pos = FVector(pos.X + nPos.X, pos.Y + nPos.Y, 0.0f);
			if (nPos == FVector(0.0f, 0.0f, 0.0f)) *stackLimit = 0;
			sum += nPos.Z;
		}
		SrcPtr = (&imageBuffer[Start - SrcWidth]);
		Value = *SrcPtr;
		if ((floor(Start / SrcWidth) != 0) && (Value > brightnessThresholdOut) && (*stackLimit == 1)) if (checkPixel[Start - SrcWidth] == 0) {
			nPos = FindBlob(SrcWidth, SrcHeight, Start - SrcWidth, checkPixel, Stack + 1, imageBuffer, stackLimit, brightnessThresholdIn, brightnessThresholdOut);
			pos = FVector(pos.X + nPos.X, pos.Y + nPos.Y, 0.0f);
			if (nPos == FVector(0.0f, 0.0f, 0.0f)) *stackLimit = 0;
			sum += nPos.Z;
		}
		SrcPtr = (&imageBuffer[Start + SrcWidth]);
		Value = *SrcPtr;
		if ((floor(Start / SrcWidth) != SrcHeight - 1) && (Value > brightnessThresholdOut) && (*stackLimit == 1)) if (checkPixel[Start + SrcWidth] == 0) {
			nPos = FindBlob(SrcWidth, SrcHeight, Start + SrcWidth, checkPixel, Stack + 1, imageBuffer, stackLimit, brightnessThresholdIn, brightnessThresholdOut);
			pos = FVector(pos.X + nPos.X, pos.Y + nPos.Y, 0.0f);
			if (nPos == FVector(0.0f, 0.0f, 0.0f)) *stackLimit = 0;
			sum += nPos.Z;
		}
	}

	if (*stackLimit == 1)
		return FVector(pos.X, pos.Y, sum);
	else
		return FVector(0.0f, 0.0f, 0.0f);

	//	return FVector(1.0f, 1.0f, 1.0f);
}

TArray<FVector> PrivateLeapImage::TrackFromLeapImage(const int32 SrcWidth, const int32 SrcHeight, uint8* imageBuffer, const uint8 brightnessThresholdIn, const uint8 brightnessThresholdOut)
{
	// Initialize search
	uint8* SrcPtr = NULL;
	FVector blob;
	int stackLimit;
	TArray<FVector> blobs2D;

	std::vector<uint8> checkPixel(SrcWidth*SrcHeight);

	for (int32 x = 0; x < SrcHeight * SrcWidth; x++)
		checkPixel[x] = 0;

	pixelsFound = 0;
	pixelLimit = 400;

	uint8 Value;
	int32 blobIndex = -1;

	for (int32 y = 0; y<SrcHeight; y++)
	{
		//		DestPtr = &MipData[(SrcHeight - 1 - y) * SrcWidth * sizeof(uint8)];
		for (int32 x = 0; x<SrcWidth; x++)
		{
			SrcPtr = (&imageBuffer[y * SrcWidth + x]);
			Value = *SrcPtr;
			if ((Value > brightnessThresholdIn) && (checkPixel[y * SrcWidth + x] == 0)) {
				pixelsFound = 0;
				stackLimit = 1;
				blob = FindBlob(SrcWidth, SrcHeight, y * SrcWidth + x, checkPixel, 0, imageBuffer, &stackLimit, brightnessThresholdIn, brightnessThresholdOut);
				if (stackLimit == 2) {
					UE_LOG(LeapPluginLog, Warning, TEXT("Blob too large!"));
				}
				if ((blob != FVector(0.f, 0.f, 0.f)) && (stackLimit == 1)) {
					blobIndex++;
					blobs2D.Add(FVector(-blob.X / blob.Z, -blob.Y / blob.Z, 1.f));
				}
			}
		}
	}

	return blobs2D;
}

//Methods below taken from http://answers.unity3d.com/questions/192261/finding-shortest-line-segment-between-two-rays-clo.html Credits to molokh
//FIND DISTANCE ALONG LINE THAT IS CLOSEST TO OTHER LINE
float PrivateLeapImage::ClosestTimeOfApproach(FVector pos1, FVector vel1, FVector pos2, FVector vel2)
{
	float t = 0;
	FVector dv = vel1 - vel2;
	float dv2 = FVector::DotProduct(dv, dv);
	if (dv2 < 0.0000001f){      // the tracks are almost parallel
		return 0.0f; // any time is ok.  Use time 0.
	}

	FVector w0 = pos1 - pos2;
	t = -(FVector::DotProduct(w0, dv)) / dv2;
	return t;
}

//FINDS DISTANCE WHERE LINES ARE CLOSEST
float PrivateLeapImage::ClosestDistOfApproach(FVector pos1, FVector vel1, FVector pos2, FVector vel2)
{
	float t = ClosestTimeOfApproach(pos1, vel1, pos2, vel2);
	FVector p1 = pos1 + (t * vel1);
	FVector p2 = pos2 + (t * vel2);

	return FVector::Dist(p1, p2);           // distance at CPA
}

//FINDS POINT ON LINE ONE CLOSEST TO LINE TWO
FVector PrivateLeapImage::ClosestPointOfApproach(FVector pos1, FVector vel1, FVector pos2, FVector vel2)
{
	float t = ClosestTimeOfApproach(pos1, vel1, pos2, vel2);
	/*	if (t<0){ // don't detect approach points in the past, only in the future;
	return (pos1);
	}
	*/
	return (((pos1 + (t * vel1)) + (pos2 + (t * vel2))) * 0.5f);
	//	return (pos2 + (t * vel2));
}

UTexture2D* PrivateLeapImage::validImagePointer(UTexture2D* pointer, int32 pWidth, int32 pHeight, EPixelFormat format)
{
	//Make sure we're valid
	if (!self->IsValid) 
	{
		UE_LOG(LeapPluginLog, Error, TEXT("Warning! Invalid Image."));
		return NULL;
	}
	//Instantiate the texture if needed
	if (pointer == NULL)
	{
		if (pWidth == 0 || pHeight == 0)
		{
			//Spit out only valid errors, two size 0 textures will be attempted per pointer,
			//unable to filter the messages out without this (or a pointer to Leap Controller, but this uses less resources).
			if (ignoreTwoInvalidSizesDone)
			{
				UE_LOG(LeapPluginLog, Error, TEXT("Warning! Leap Image SDK access is denied, please enable image support from the Leap Controller before events emit (e.g. at BeginPlay)."));
			}
			else
			{
				invalidSizesReported++;

				if (invalidSizesReported == 2)
				{
					ignoreTwoInvalidSizesDone = true;
				}
			}
			
			return NULL;
		}
		UE_LOG(LeapPluginLog, Log, TEXT("Created Leap Image Texture Sized: %d, %d."), pWidth, pHeight);
		pointer = UTexture2D::CreateTransient(pWidth, pHeight, format); //PF_B8G8R8A8
		pointer->SetFlags(RF_RootSet);	//to support more than one leap component, the pointer shouldn't be reclaimed by GC
	}
	//If the size changed, recreate the image
	if (pointer->PlatformData->SizeX != pWidth ||
		pointer->PlatformData->SizeY != pHeight)
	{
		UE_LOG(LeapPluginLog, Log, TEXT("ReCreated Leap Image Texture Sized: %d, %d. Old Size: %d, %d"), pWidth, pHeight, pointer->PlatformData->SizeX, pointer->PlatformData->SizeY);
		//pointer->ConditionalBeginDestroy();
		pointer = UTexture2D::CreateTransient(pWidth, pHeight, format);
	}
	return pointer;
}

//Single Channel texture
UTexture2D* PrivateLeapImage::Texture8FromLeapImage(int32 SrcWidth, int32 SrcHeight, uint8* imageBuffer)
{
	// Lock the texture so it can be modified
	if (imageR8Pointer == NULL)
		return NULL;

	// Lock the texture so it can be modified
	uint8* MipData = static_cast<uint8*>(imageR8Pointer->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	// Create base mip.
	uint8* DestPtr = NULL;
	uint8* SrcPtr = NULL;

	for (int32 y = 0; y<SrcHeight; y++)
	{
		DestPtr = &MipData[(SrcHeight - 1 - y) * SrcWidth * sizeof(uint8)];
		SrcPtr = (&imageBuffer[(SrcHeight - 1 - y) * SrcWidth]);
		for (int32 x = 0; x<SrcWidth; x++)
		{
			*DestPtr++ = *SrcPtr;
			SrcPtr++;
		}
	}

	// Unlock the texture
	imageR8Pointer->PlatformData->Mips[0].BulkData.Unlock();
	imageR8Pointer->UpdateResource();

	return imageR8Pointer;
}

//Grayscale average texture
UTexture2D* PrivateLeapImage::Texture32FromLeapImage(int32 SrcWidth, int32 SrcHeight, uint8* imageBuffer)
{
	// Lock the texture so it can be modified
	if (imagePointer == NULL)
		return NULL;

	uint8* MipData = static_cast<uint8*>(imagePointer->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));
	
	// Create base mip.
	uint8* DestPtr = NULL;
	const uint8* SrcPtr = NULL;

	for (int32 y = 0; y<SrcHeight; y++)
	{
		DestPtr = &MipData[(SrcHeight - 1 - y) * SrcWidth * sizeof(FColor)];
		SrcPtr = const_cast<uint8*>(&imageBuffer[(SrcHeight - 1 - y) * SrcWidth]);
		for (int32 x = 0; x<SrcWidth; x++)
		{
			//Grayscale, copy to all channels
			*DestPtr++ = *SrcPtr;
			*DestPtr++ = *SrcPtr;
			*DestPtr++ = *SrcPtr;
			*DestPtr++ = 0xFF;
			SrcPtr++;
		}
	}

	// Unlock the texture
	imagePointer->PlatformData->Mips[0].BulkData.Unlock();
	imagePointer->UpdateResource();

	return imagePointer;
}

/*Lossless distortion map attempts so far no good
UTexture2D* PrivateLeapImage::TextureFFromLeapDistortion(int32 SrcWidth, int32 SrcHeight, float* imageBuffer)
{
	int32 DestWidth = SrcWidth/2; // Put 2 floats in the R and G channels
	int32 DestHeight = SrcHeight;


	// Create the texture
	UTexture2D* LeapTexture = UTexture2D::CreateTransient(DestWidth, DestHeight, PF_A32B32G32R32F);

	// Lock the texture so it can be modified
	float* MipData = static_cast<float*>(LeapTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	// Create base mip.
	float* DestPtr = NULL;
	const float* SrcPtr = NULL;
	for (int32 y = 0; y<SrcHeight; y++)
	{
		DestPtr = &MipData[(DestHeight - 1 - y) * DestWidth * sizeof(float)*2];
		SrcPtr = const_cast<float*>(&imageBuffer[(SrcHeight - 1 - y) * SrcWidth]);
		for (int32 x = 0; x<SrcWidth; x++)
		{
			*DestPtr++ = 1.f;						//Full 255 alpha
			*DestPtr++ = 0.f;						//Black
			*DestPtr++ = (float)*SrcPtr++;//(uint8)(*SrcPtr++ * 0xFF); //Scale the float (0.0..1.0) to a uint8 (0..255)
			*DestPtr++ = (float)*SrcPtr++;//(uint8)(*SrcPtr++ * 0xFF);
		}
	}

	// Unlock the texture
	LeapTexture->PlatformData->Mips[0].BulkData.Unlock();
	LeapTexture->UpdateResource();

	return LeapTexture;
}*/

UTexture2D* PrivateLeapImage::Texture32FromLeapDistortion(int32 SrcWidth, int32 SrcHeight, float* imageBuffer)
{
	// Lock the texture so it can be modified
	if (distortionPointer == NULL)
		return NULL;

	int32 DestWidth = SrcWidth /2; // Put 2 floats in the R and G channels
	int32 DestHeight = SrcHeight;

	// Lock the texture so it can be modified
	uint8* MipData = static_cast<uint8*>(distortionPointer->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE));

	// Create base mip.
	uint8* DestPtr = NULL;
	const float* SrcPtr = NULL;
	for (int32 y = 0; y<SrcHeight; y++)
	{
		DestPtr = &MipData[(DestHeight - 1 - y) * DestWidth * sizeof(FColor)];
		SrcPtr = const_cast<float*>(&imageBuffer[(SrcHeight - 1 - y) * SrcWidth]);
		for (int32 x = 0; x<SrcWidth; x++)
		{

			*DestPtr++ = (uint8)(*SrcPtr++ * 0xFF);	//Scale floats
			*DestPtr++ = (uint8)(*SrcPtr++ * 0xFF);
			*DestPtr++ = (uint8)0x00;						//Black
			*DestPtr++ = (uint8)0xFF;						//Full 255 alpha
		}
	}

	// Unlock the texture
	distortionPointer->PlatformData->Mips[0].BulkData.Unlock();
	distortionPointer->UpdateResource();

	return distortionPointer;
}

UTexture2D* ULeapImage::Texture()
{
	_private->imagePointer = _private->validImagePointer(_private->imagePointer, Width, Height, PF_B8G8R8A8);
	
	return _private->Texture32FromLeapImage(Width, Height, (uint8*)_private->leapImage.data());
}

UTexture2D* ULeapImage::R8Texture()
{
	_private->imageR8Pointer = _private->validImagePointer(_private->imageR8Pointer, Width, Height, PF_G8);

	return _private->Texture8FromLeapImage(Width, Height, (uint8*)_private->leapImage.data());
}

UTexture2D* ULeapImage::Distortion()
{
	_private->distortionPointer = _private->validImagePointer(_private->distortionPointer, DistortionWidth / 2, DistortionHeight, PF_B8G8R8A8);

	return _private->Texture32FromLeapDistortion(DistortionWidth, DistortionHeight, (float*)_private->leapImage.distortion());
}

TArray<FVector> ULeapImage::TrackBlobs(uint8 brightnessThresholdIn, uint8 brightnessThresholdOut)
{
	return _private->TrackFromLeapImage(Width, Height, (uint8*)_private->leapImage.data(), brightnessThresholdIn, brightnessThresholdOut);
}

TArray<FVector> ULeapImage::LocateBlobs(TArray<FVector> blobArray1, TArray<FVector> blobArray2)
{
	TArray<FVector> BlobVectors;
	uint8 matchBlobs[200];
	FVector blobLocation, blob1, blob2, cameraOffset;;

	cameraOffset = FVector(-0.02f, 0.0f, 0.0f);

	//MATCH THE LEFT RAYS TO THE RIGHT RAYS
	//MAKE SURE RAYS ARE AT LEAST WITHIN 0.05 OF EACHOTHER
	//PICK PAIR THAT RESULTS IN LONGER DISTANCE TO AVOID ENTANGLEMENT
	for (int i = 0; blobArray1.IsValidIndex(i); i++) {
		matchBlobs[i] = 255;
	}

	float farthest;
	for (int i = 0; blobArray1.IsValidIndex(i); i++){

		farthest = 0.05f; //MUST BE FARTHER THAN THIS TO COUNT AT ALL
		for (int j = 0; blobArray2.IsValidIndex(j); j++){

			//TEST ALL FOR FARTHEST WORKING BLOB
			float intersectiondistance = _private->ClosestDistOfApproach(-cameraOffset, blobArray1[i], cameraOffset, blobArray2[j]);
			float distancefromleap = _private->ClosestTimeOfApproach(-cameraOffset, blobArray1[i], cameraOffset, blobArray2[j]);
			if (distancefromleap > farthest) {
				if (intersectiondistance < 0.02f) {
					farthest = distancefromleap;
					matchBlobs[i] = j;
				}
			}
		}
	}

	//ADD BLOB POSITIONS (RELATIVE TO PARENT) ONLY AT THE INTERSECTION OF THE MATCHED RAYS
	for (int i = 0; blobArray1.IsValidIndex(i); i++){
		if (matchBlobs[i] != 255){
			blobLocation = _private->ClosestPointOfApproach(-cameraOffset, blobArray1[i], cameraOffset, blobArray2[matchBlobs[i]]);
			BlobVectors.Add(blobLocation);
		}
	}

	return BlobVectors;
}

FVector ULeapImage::Rectify(FVector uv) const
{
	Leap::Vector vect = Leap::Vector(uv.X, uv.Y, uv.Z);
	vect = _private->leapImage.rectify(vect);
	return FVector(vect.x, vect.y, vect.z);
}

FVector ULeapImage::Warp(FVector xy) const
{
	Leap::Vector vect = Leap::Vector(xy.X, xy.Y, xy.Z);
	vect = _private->leapImage.warp(vect);
	return FVector(vect.x, vect.y, vect.z);
}

void ULeapImage::setLeapImage(const Leap::Image &LeapImage)
{
	_private->leapImage = LeapImage;

	DistortionHeight = _private->leapImage.distortionHeight();
	DistortionWidth = _private->leapImage.distortionWidth();
	Height = _private->leapImage.height();
	Id = _private->leapImage.id();
	IsValid = _private->leapImage.isValid();
	RayOffsetX = _private->leapImage.rayOffsetX();
	RayOffsetY = _private->leapImage.rayOffsetY();
	RayScaleX = _private->leapImage.rayScaleX();
	RayScaleY = _private->leapImage.rayScaleY();
	Width = _private->leapImage.width();
}