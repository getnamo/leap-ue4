#pragma once

#include "LeapMotionPublicPCH.h"
#include "LeapImage.generated.h"


UCLASS(BlueprintType)
class ULeapImage : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	~ULeapImage();

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Texture", CompactNodeTitle = "", Keywords = "get texture"), Category = "Leap Image")
	class UTexture2D* Texture();

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "R8Texture", CompactNodeTitle = "", Keywords = "get texture single channel"), Category = "Leap Image")
	class UTexture2D* R8Texture();
	
	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "Distortion", CompactNodeTitle = "", Keywords = "distortion"), Category = "Leap Image")
	class UTexture2D* Distortion();

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "TrackBlobs", CompactNodeTitle = "", Keywords = "blob tracking"), Category = "Leap Image")
	TArray<FVector> TrackBlobs(uint8 brightnessThresholdIn, uint8 brightnessThresholdOut);

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "LocateBlobs", CompactNodeTitle = "", Keywords = "blob location"), Category = "Leap Image")
	TArray<FVector> LocateBlobs(TArray<FVector> blobArray1, TArray<FVector> blobArray2);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	int32 DistortionHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	int32 DistortionWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	bool IsValid;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	float RayOffsetX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	float RayOffsetY;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	float RayScaleX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	float RayScaleY;

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "rectify", CompactNodeTitle = "", Keywords = "rectify"), Category = "Leap Image")
	FVector Rectify(FVector uv) const;

	UFUNCTION(BlueprintCallable, meta = (FriendlyName = "warp", CompactNodeTitle = "", Keywords = "warp"), Category = "Leap Image")
	FVector Warp(FVector xy) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Image")
	int32 Width;

	void setLeapImage(const class Leap::Image &LeapImage);

	virtual void CleanupRootReferences();

private:
	class PrivateLeapImage* _private;
};