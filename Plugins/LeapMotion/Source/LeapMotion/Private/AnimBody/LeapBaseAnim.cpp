// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapMotionPrivatePCH.h"
#include "LeapBaseAnim.h"

ULeapBaseAnim::ULeapBaseAnim(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void ULeapBaseAnim::InitializeAnimBodyIfNew()
{
	// Cast to Character
	ACharacter* CharacterCast = Cast<ACharacter>(this->OwningPawn);

	if (CharacterCast)
	{
		// If character changed or AnimBody invalid, reset
		if (Character != CharacterCast || !AnimBody)
		{
			Character = CharacterCast;
			// Does it implement the anim body interface? 

			IAnimBodyInterface* AnimBodyInterface = Cast<IAnimBodyInterface>(Character);
			if (AnimBodyInterface)
			{
				//UE_LOG(FantasiaInit, Log, TEXT("ULeapBaseAnim AnimBodyInterface has been found in pawn: "), *Character->GetName());
				AnimBody = AnimBodyInterface->GetAnimBody();
			}
			else
			{
				//UE_LOG(FantasiaInit, Warning, TEXT("ULeapBaseAnim AnimBodyInterface is not implemented in pawn: "), *Character->GetName());
				AnimBody = nullptr;
			}
		}
	}
	else
	{
		//UE_LOG(FantasiaInit, Warning, TEXT("ULeapBaseAnim Failed to cast Character!"));
	}
}



void ULeapBaseAnim::SetScalingIfEnabled()
{
	if (IsScalingSetTest)
	{
		if (LeftIKAlphaTest == 1.f)
		{
			// Use Left Hand / Arm To determine scaling
			float distance = DistanceBetweenVectors(LeftHandTransformTest.GetLocation(), LeftElbowTransformTest.GetLocation());
			if (distance > 0.f)
			{
				float distanceBetweenBones = DistanceBetweenBones("lowerarm_l", "hand_l");
				ArmScalingTest = distance / distanceBetweenBones;
				IsScalingSetTest = 1.f;
				//UE_LOG(FantasiaInit, Log, TEXT("ULeapBaseAnim Arm Scaling Factor: %f"), ArmScalingTest);
			}
		}
	}
}



float ULeapBaseAnim::DistanceBetweenVectors(FVector A, FVector B)
{
	return (A - B).Size();
}

float ULeapBaseAnim::DistanceBetweenBones(FName Bone1, FName Bone2)
{
	return DistanceBetweenVectors(Character->GetMesh()->GetSocketLocation(Bone1), Character->GetMesh()->GetSocketLocation(Bone2));
}

FRotator ULeapBaseAnim::ChangeRotationBase(FRotator In, FRotator PreBase, FRotator PostBase)
{
	return UKismetMathLibrary::ComposeRotators(PreBase, UKismetMathLibrary::ComposeRotators(In, PostBase));
}

FRotator ULeapBaseAnim::CompensateRotation(FRotator In)
{
	float Roll, Pitch, Yaw;
	UKismetMathLibrary::BreakRotator(In, Roll, Pitch, Yaw);
	return UKismetMathLibrary::ComposeRotators(FRotator(-90.f, 0.f, 90.f), UKismetMathLibrary::MakeRotator(-1.f*Pitch, Roll, Yaw));
}

FTransform ULeapBaseAnim::CompensateTransformForYForward(FTransform In, FRotator PreBase, FRotator PostBase)
{
	FVector Location, Scale;
	FRotator Rotation;
	UKismetMathLibrary::BreakTransform(In, Location, Rotation, Scale);
	return UKismetMathLibrary::MakeTransform(
		CompensateVectorForYForward(Location),
		ChangeRotationBase(Rotation, PreBase, PostBase),
		Scale);
}

FVector ULeapBaseAnim::CompensateVectorForYForward(FVector In)
{
	return FRotator(0.f, 0.f, 90.f).RotateVector(In);
}

void ULeapBaseAnim::SetLocalVariableFromAnimBody()
{
	
	// Convenience Pointers
	LeftHandTest = AnimBody->Left;
	RightHandTest = AnimBody->Right;

	// Alpha
	LeftHandTest->Alpha = LeftIKAlphaTest;
	RightHandTest->Alpha = RightIKAlphaTest;

	// Head Variables
	HeadOrientationTest = AnimBody->Head->Orientation;
	HeadPositionTest = AnimBody->Head->Position;
	HeadAlphaTest = AnimBody->Head->Alpha;
	float Roll, Pitch, Yaw;
	FRotator rot = UKismetMathLibrary::ComposeRotators(AnimBody->Head->Orientation, FRotator(0.f, 0.f, -90.f));
	UKismetMathLibrary::BreakRotator(rot, Roll, Pitch, Yaw);
	Yaw = UKismetMathLibrary::ClampAngle(Yaw / 2.f, -40.f, 40.f);
	ShoulderOrientationTest = UKismetMathLibrary::MakeRotator(0.f, 0.f, Yaw);

	// Hand
	LeftHandTransformTest = LeftHandTest->Wrist->GetTransform();
	RightHandTransformTest = RightHandTest->Wrist->GetTransform();
	//UE_LOG(FantasiaInit, Log, TEXT("ULeapBaseAnim RightHandTransformTest: %s     %d "), *RightHandTransformTest.GetLocation().ToString(), this);

	// Elbow/LowerArm
	LeftElbowTransformTest = LeftHandTest->LowerArm->GetTransform();
	RightElbowTransformTest = RightHandTest->LowerArm->GetTransform();

	// Left Fingers
	// Right Fingers
	// TODO Finish the implementation for fingers (make a function taking a hand in paramater to avoid duplicate code
	SetFinger(LeftHandTest->Index, &LeftIndexOrientation1, &LeftIndexOrientation2, &LeftIndexOrientation3);
	SetFinger(LeftHandTest->Middle, &LeftMiddleOrientation1, &LeftMiddleOrientation2, &LeftMiddleOrientation3);
	SetFinger(LeftHandTest->Ring, &LeftRingOrientation1, &LeftRingOrientation2, &LeftRingOrientation3);
	SetFinger(LeftHandTest->Pinky, &LeftPinkyOrientation1, &LeftPinkyOrientation2, &LeftPinkyOrientation3);
	SetFinger(LeftHandTest->Thumb, &LeftThumbOrientation1, &LeftThumbOrientation2, &LeftThumbOrientation3);

	SetFinger(RightHandTest->Index, &RightIndexOrientation1, &RightIndexOrientation2, &RightIndexOrientation3);
	SetFinger(RightHandTest->Middle, &RightMiddleOrientation1, &RightMiddleOrientation2, &RightMiddleOrientation3);
	SetFinger(RightHandTest->Ring, &RightRingOrientation1, &RightRingOrientation2, &RightRingOrientation3);
	SetFinger(RightHandTest->Pinky, &RightPinkyOrientation1, &RightPinkyOrientation2, &RightPinkyOrientation3);
	SetFinger(RightHandTest->Thumb, &RightThumbOrientation1, &RightThumbOrientation2, &RightThumbOrientation3);

}


void ULeapBaseAnim::SetFinger(UAnimFinger* Finger, FRotator* Orientation1, FRotator* Orientation2, FRotator* Orientation3)
{
	*Orientation1 = Finger->Proximal->Orientation;
	*Orientation2 = Finger->Intermediate->Orientation;
	*Orientation3 = Finger->Distal->Orientation;
}

void ULeapBaseAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	//Very Important Line
	Super::NativeUpdateAnimation(DeltaSeconds);

	//Always Check Pointers
	if (OwningPawn)
	{
		// Set 'IsInAir' (used in state machine)
		IsInairTest = OwningPawn->GetMovementComponent()->IsFalling();
		// Setting 'Speed' (use in 1D blend space)
		Speed = OwningPawn->GetVelocity().Size();

		InitializeAnimBodyIfNew();

		if (AnimBody)
		{
			//UE_LOG(FantasiaInit, Log, TEXT("ULeapBaseAnim AnimBody has been found."));
			// Is our body enabled? (alpha 1.0)
			if (AnimBody->Enabled() == 1.f)
			{
				//UE_LOG(FantasiaInit, Log, TEXT("ULeapBaseAnim AnimBody is enabled."));
				SetLocalVariableFromAnimBody();
				SetScalingIfEnabled();
			}
			else
			{
				//Ensure our IK alpha reflects anim body state by disabling alpha
				LeftIKAlphaTest = 0.0f;
				RightIKAlphaTest = 0.0f;
			}
		}
		else
		{
			//UE_LOG(FantasiaInit, Warning, TEXT("ULeapBaseAnim AnimBody has not been found."));
		}
	}
	else
	{
		//Cache the owning pawn for use in Tick
		OwningPawn = TryGetPawnOwner();
	}
}


bool ULeapBaseAnim::NativeEvaluateAnimation(FPoseContext& Output)
{
	USkeletalMeshComponent* OwningComponent = GetOwningComponent();

	//UE_LOG(FantasiaInit, Warning, TEXT("NativeEvaluateAnimation Check for OwningComponent: %s"), *OwningComponent->GetName());
	if (OwningComponent)
	{
		int32 BoneIndex = OwningComponent->GetBoneIndex("index_03_l");
		const FBoneContainer BoneContainer = Output.Pose.GetBoneContainer(); // Get all bones here
	}

	return Super::NativeEvaluateAnimation(Output);
}
