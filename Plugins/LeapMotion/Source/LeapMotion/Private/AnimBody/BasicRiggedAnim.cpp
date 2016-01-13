// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapMotionPrivatePCH.h"
#include "BasicRiggedAnim.h"


UBasicRiggedAnim::UBasicRiggedAnim(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{

}

FVector UBasicRiggedAnim::VectorBetweenBones(FName Bone1, FName Bone2)
{
	return Character->GetMesh()->GetSocketLocation(Bone1) - Character->GetMesh()->GetSocketLocation(Bone2);
}

void UBasicRiggedAnim::DetermineWristBoneOffset()
{
	FVector Location, Scale;
	FRotator Rotation;
	UKismetMathLibrary::BreakTransform(LeftElbowTransformTest, Location, Rotation, Scale);
	float VectorLength = VectorBetweenBones("lf_forearm_roll_d", "lf_wrist").Size();
	LWristBoneOffset = VectorLength * Rotation;

	UKismetMathLibrary::BreakTransform(RightElbowTransformTest, Location, Rotation, Scale);
	VectorLength = VectorBetweenBones("rt_forearm_roll_d", "rt_wrist").Size();
	RWristBoneOffset = VectorLength * Rotation;
}

void UBasicRiggedAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	ULeapBaseAnim::NativeUpdateAnimation(DeltaSeconds);
	if (OwningPawn && AnimBody)
	{
		DetermineWristBoneOffset();
	}
}