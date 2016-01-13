// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeapBaseAnim.h"
#include "BasicRiggedAnim.generated.h"

/**
 * 
 */
UCLASS()
class LEAPMOTION_API UBasicRiggedAnim : public ULeapBaseAnim
{

public:

	void DebugAdjustFingerOrientationRollForHand(UAnimHand* Hand, FRotator AdditionalRotation);
	UBasicRiggedAnim(const FObjectInitializer & ObjectInitializer);
	FVector VectorBetweenBones(FName Bone1, FName Bone2);
	void DetermineWristBoneOffset();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	FRotator LWristBoneOffset;
	FRotator RWristBoneOffset;
	GENERATED_BODY()

};
