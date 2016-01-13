// Created by Chance_07 2014
#pragma once

#include "LeapMotionPublicPCH.h"
#include "Runtime/AnimGraphRuntime/Public/BoneControllers/AnimNode_ModifyBone.h"
#include "AnimNode_ModifyFinger.generated.h"


USTRUCT()
struct LEAPMOTION_API FAnimNode_ModifyFinger : public  FAnimNode_SkeletalControlBase
{

	GENERATED_USTRUCT_BODY()
		
	///** Name of bone to control **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference TargetFingerBone1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference TargetFingerBone2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalControl)
		FBoneReference TargetFingerBone3;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation, meta = (PinShownByDefault))
		FRotator Rotation1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation, meta = (PinShownByDefault))
		FRotator Rotation2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation, meta = (PinShownByDefault))
		FRotator Rotation3;

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


	FAnimNode_ModifyFinger();

	// FAnimNode_Base interface
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;
	// End of FAnimNode_Base interface

	// FAnimNode_SkeletalControlBase interface
	virtual void EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms) override;

	void EvaluateOneBoneTransforms(FBoneReference* BoneToModify, FRotator* Rotation, const FBoneContainer BoneContainer, FCSPose<FCompactPose> &MeshBases, USkeletalMeshComponent* SkelComp, TArray<FBoneTransform> &OutBoneTransforms);

	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface

private:
	// FAnimNode_SkeletalControlBase interface
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	// End of FAnimNode_SkeletalControlBase interface



public:
	// FAnimNode_Base interface
	/*virtual void Initialize(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones(const FAnimationCacheBonesContext & Context)  override;
	virtual void Update(const FAnimationUpdateContext& Context) override;
	virtual void EvaluateComponentSpace(FComponentSpacePoseContext& Output) override;*/
	// End of FAnimNode_RCAnimNode interface
protected:
	// initialize any bone references you have
	//virtual void InitializeBoneReferences(const FBoneContainer & RequiredBones);
	//// return true if it is valid to Evaluate
	//virtual bool IsValidToEvaluate(const USkeleton * Skeleton, const FBoneContainer & RequiredBones);
	//// Evaluate the new component-space transforms for the affected bones.
	//virtual void EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, const FBoneContainer & RequiredBones, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms);// {}
private:
	//FVector ModTargetLocationInCompSpace;
	//FVector CurrentAddedOffset;
	//FVector JointOffset;
};