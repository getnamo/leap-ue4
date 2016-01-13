// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "AnimBody.h"
#include "Animation/AnimNodeSpaceConversions.h"
#include "AnimBodyInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "LeapBaseAnim.generated.h"

/**
 * TODO write summary and set function blueprint callable.
 */
UCLASS()
class LEAPMOTION_API ULeapBaseAnim : public UAnimInstance
{
private:
	GENERATED_BODY()

protected:
	ACharacter* Character;
	APawn* OwningPawn;

	void SetFinger(UAnimFinger* Finger, FRotator* Orientation1, FRotator* Orientation2, FRotator* Orientation3);

public:

	ULeapBaseAnim(const FObjectInitializer & ObjectInitializer);

	// IE the anim graph update event (kinda)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual bool NativeEvaluateAnimation(FPoseContext& Output) override;

	void InitializeAnimBodyIfNew();
	void SetLocalVariableFromAnimBody();
	void SetScalingIfEnabled();
	static float DistanceBetweenVectors(FVector A, FVector B);
	float DistanceBetweenBones(FName Bone1, FName Bone2);
	FRotator ChangeRotationBase(FRotator In, FRotator PreBase, FRotator PostBase);
	FRotator CompensateRotation(FRotator In);
	FTransform CompensateTransformForYForward(FTransform In, FRotator PreBase, FRotator PostBase);
	FVector CompensateVectorForYForward(FVector In);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftIndexOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftIndexOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftIndexOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftMiddleOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftMiddleOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftMiddleOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftRingOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftRingOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftRingOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftPinkyOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftPinkyOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftPinkyOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftThumbOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftThumbOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator LeftThumbOrientation3;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightIndexOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightIndexOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightIndexOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightMiddleOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightMiddleOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightMiddleOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightRingOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightRingOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightRingOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightPinkyOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightPinkyOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightPinkyOrientation3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightThumbOrientation1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightThumbOrientation2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		FRotator RightThumbOrientation3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		float LeftIKAlphaTest = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		float RightIKAlphaTest = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FRotator HeadOrientationTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FVector HeadPositionTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		float HeadAlphaTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FRotator ShoulderOrientationTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FTransform LeftHandTransformTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FTransform RightHandTransformTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FTransform LeftElbowTransformTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		FTransform RightElbowTransformTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		float ArmScalingTest = 1.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		float IsScalingSetTest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		bool IsInairTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocalAnim")
		float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAnimHand* LeftHandTest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAnimHand* RightHandTest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAnimBody* AnimBody;
};
