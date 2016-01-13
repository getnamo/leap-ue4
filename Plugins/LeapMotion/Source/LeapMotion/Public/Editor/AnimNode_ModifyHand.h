// Created by Chance_07 2014
#pragma once

#include "LeapMotionPublicPCH.h"
#include "Runtime/AnimGraphRuntime/Public/BoneControllers/AnimNode_ModifyBone.h"
#include "AnimHand.h"
#include "AnimNode_ModifyHand.generated.h"


USTRUCT()
struct LEAPMOTION_API FAnimNode_ModifyHand : public  FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()


	struct SFingerBonesReferences
	{
		FBoneReference* Proximal;
		FBoneReference* Intermediate;
		FBoneReference* Distal;
	};

	struct SHandBonesReferences
	{
		FBoneReference* All[15];
		SFingerBonesReferences Thumb;
		SFingerBonesReferences Index;
		SFingerBonesReferences Middle;
		SFingerBonesReferences Ring;
		SFingerBonesReferences Pinky;
	};

	///** Name of bone to control **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneThumb1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneThumb2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneThumb3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneIndex1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneIndex2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneIndex3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneMiddle1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneMiddle2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneMiddle3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneRing1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneRing2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BoneRing3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BonePinky1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BonePinky2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference BonePinky3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hand, meta = (PinShownByDefault))
		UAnimHand* Hand;

	FVector Scale;

	EBoneModificationMode RotationMode;
	EBoneControlSpace RotationSpace;
	EBoneModificationMode TranslationMode;
	EBoneModificationMode ScaleMode;
	EBoneControlSpace TranslationSpace;
	EBoneControlSpace ScaleSpace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation, meta = (PinShownByDefault))
	FVector Translation1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation, meta = (PinShownByDefault))
		FVector Translation2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation, meta = (PinShownByDefault))
	FVector Translation3;

	FAnimNode_ModifyHand();

	void InitHandBonesReferences();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms) override;

	void EvaluateHandBonesTransforms(
		SHandBonesReferences* HandBonesReferences,
		UAnimHand* Hand,
		const FBoneContainer BoneContainer,
		FCSPose<FCompactPose>& MeshBases,
		USkeletalMeshComponent* SkelComp,
		TArray<FBoneTransform>& OutBoneTransforms);

	void EvaluateFingerBonesTransforms(
		SFingerBonesReferences* FingerBonesReferences,
		UAnimFinger* Finger,
		const FBoneContainer BoneContainer,
		FCSPose<FCompactPose>& MeshBases,
		USkeletalMeshComponent* SkelComp,
		TArray<FBoneTransform>& OutBoneTransforms);

	void EvaluateOneBoneTransforms(
		FBoneReference* BoneToModify,
		FRotator* Rotation,
		const FBoneContainer BoneContainer,
		FCSPose<FCompactPose> &MeshBases,
		USkeletalMeshComponent* SkelComp,
		TArray<FBoneTransform> &OutBoneTransforms);

	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

	SHandBonesReferences* HandBonesReferences;
private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

};