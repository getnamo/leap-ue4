#include "LeapMotionPrivatePCH.h"
#include "AnimNode_ModifyFinger.h"


FAnimNode_ModifyFinger::FAnimNode_ModifyFinger()
	: FAnimNode_SkeletalControlBase()
	//, bUpdateX(false)// This is only to set
	//, bUpdateY(false)// the defaults for the
	//, bUpdateZ(true)// Axis pins (Can be changed in BP)
	//, AddtoOffset(FVector::ZeroVector)
{
	Translation1 = FVector::ZeroVector;
	Translation2 = FVector::ZeroVector;
	Translation3 = FVector::ZeroVector;
	//Rotation = FRotator::ZeroRotator; 
	Scale = FVector(1.0f);
	TranslationMode = BMM_Ignore;
	RotationMode = BMM_Replace; // Replace current rotation when modifying
	Alpha = 1.f;
	ScaleMode = BMM_Ignore;
	TranslationSpace = BCS_WorldSpace;
	RotationSpace = BCS_ComponentSpace;
	ScaleSpace = BCS_ComponentSpace;
	//BlendWeight = 1;
	//JointOffset.ZeroVector;
	// NULL so that it will be tested at least once
	//CurrentAddedOffset.X = NULL;
	//CurrentAddedOffset.Y = NULL;
	//CurrentAddedOffset.Z = NULL;
}



void FAnimNode_ModifyFinger::GatherDebugData(FNodeDebugData& DebugData)
{
	FString DebugLine = DebugData.GetNodeName(this);

	DebugLine += "(";
	AddDebugNodeData(DebugLine);
	DebugLine += FString::Printf(TEXT(" Target: %s)"), *TargetFingerBone1.BoneName.ToString());
	DebugLine += FString::Printf(TEXT(" Target: %s)"), *TargetFingerBone2.BoneName.ToString());
	DebugLine += FString::Printf(TEXT(" Target: %s)"), *TargetFingerBone3.BoneName.ToString());
	DebugData.AddDebugItem(DebugLine);

	ComponentPose.GatherDebugData(DebugData);
}

void FAnimNode_ModifyFinger::EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms)
{
	check(OutBoneTransforms.Num() == 0);

	// the way we apply transform is same as FMatrix or FTransform
	// we apply scale first, and rotation, and translation
	// if you'd like to translate first, you'll need two nodes that first node does translate and second nodes to rotate.
	const FBoneContainer BoneContainer = MeshBases.GetPose().GetBoneContainer();
	
	// For all bones
	EvaluateOneBoneTransforms(&TargetFingerBone1, &Rotation1, BoneContainer, MeshBases, SkelComp, OutBoneTransforms);
	EvaluateOneBoneTransforms(&TargetFingerBone2, &Rotation2, BoneContainer, MeshBases, SkelComp, OutBoneTransforms);
	EvaluateOneBoneTransforms(&TargetFingerBone3, &Rotation3, BoneContainer, MeshBases, SkelComp, OutBoneTransforms);
}

void FAnimNode_ModifyFinger::EvaluateOneBoneTransforms(FBoneReference* BoneToModify, FRotator* Rotation, const FBoneContainer BoneContainer, FCSPose<FCompactPose> &MeshBases, USkeletalMeshComponent* SkelComp, TArray<FBoneTransform> &OutBoneTransforms)
{
	FCompactPoseBoneIndex CompactPoseBoneToModify = BoneToModify->GetCompactPoseIndex(BoneContainer);
	FTransform NewBoneTM = MeshBases.GetComponentSpaceTransform(CompactPoseBoneToModify);

	if (RotationMode != BMM_Ignore)
	{
		// Convert to Bone Space.
		FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp, MeshBases, NewBoneTM, CompactPoseBoneToModify, RotationSpace);

		const FQuat BoneQuat(*Rotation);
		if (RotationMode == BMM_Additive)
		{
			NewBoneTM.SetRotation(BoneQuat * NewBoneTM.GetRotation());
		}
		else
		{
			NewBoneTM.SetRotation(BoneQuat);
		}

		// Convert back to Component Space.
		FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp, MeshBases, NewBoneTM, CompactPoseBoneToModify, RotationSpace);
	}

	
	if (ScaleMode != BMM_Ignore)
	{
		// Convert to Bone Space.
		FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp, MeshBases, NewBoneTM, CompactPoseBoneToModify, ScaleSpace);

		if (ScaleMode == BMM_Additive)
		{
			NewBoneTM.SetScale3D(NewBoneTM.GetScale3D() * Scale);
		}
		else
		{
			NewBoneTM.SetScale3D(Scale);
		}

		// Convert back to Component Space.
		FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp, MeshBases, NewBoneTM, CompactPoseBoneToModify, ScaleSpace);
	}
	
	
	if (TranslationMode != BMM_Ignore)
	{
		// Convert to Bone Space.
		FAnimationRuntime::ConvertCSTransformToBoneSpace(SkelComp, MeshBases, NewBoneTM, CompactPoseBoneToModify, TranslationSpace);

		if (TranslationMode == BMM_Additive)
		{
			NewBoneTM.AddToTranslation(Translation1);
		}
		else
		{
			NewBoneTM.SetTranslation(Translation1);
		}

		// Convert back to Component Space.
		FAnimationRuntime::ConvertBoneSpaceTransformToCS(SkelComp, MeshBases, NewBoneTM, CompactPoseBoneToModify, TranslationSpace);
	}
	OutBoneTransforms.Add(FBoneTransform(BoneToModify->GetCompactPoseIndex(BoneContainer), NewBoneTM));

	const float BlendWeight = FMath::Clamp<float>(Alpha, 0.f, 1.f);
	MeshBases.LocalBlendCSBoneTransforms(OutBoneTransforms, BlendWeight);
	OutBoneTransforms.Empty();
}

bool FAnimNode_ModifyFinger::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	// if both bones are valid
	return TargetFingerBone1.IsValid(RequiredBones)
		&& TargetFingerBone2.IsValid(RequiredBones)
		&& TargetFingerBone3.IsValid(RequiredBones);
}

void FAnimNode_ModifyFinger::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	// BoneToModify.Initialize(RequiredBones); 
	TargetFingerBone1.Initialize(RequiredBones);
	TargetFingerBone2.Initialize(RequiredBones);
	TargetFingerBone3.Initialize(RequiredBones);
}
































/*
void FAnimNode_ModifyFinger::Initialize(const FAnimationInitializeContext& Context)
{
	ComponentPose.Initialize(Context);
}

void FAnimNode_ModifyFinger::Update(const FAnimationUpdateContext& Context)
{
	ComponentPose.Update(Context);
}

void FAnimNode_ModifyFinger::CacheBones(const FAnimationCacheBonesContext & Context)
{
	InitializeBoneReferences(Context.AnimInstance->RequiredBones);
	ComponentPose.CacheBones(Context);
}

//This is just a check to make sure that the bones are present
void FAnimNode_ModifyFinger::InitializeBoneReferences(const FBoneContainer & RequiredBones)
{
	// Check the Source Bone
	if (SourceBone.Initialize(RequiredBones)) {}
	else
	{
		UE_LOG(LogAnimation, Warning, TEXT("No Source Bone found, please check input"));
	}

	// Check the Target Bone
	if (TargetBone.Initialize(RequiredBones)) {}
	else
	{
		UE_LOG(LogAnimation, Warning, TEXT("No Target Bone found, please check input"));
	}
}

//This Checks for Updates, if no Axis is checked, returns Base Pose and moves on
void FAnimNode_ModifyFinger::EvaluateComponentSpace(FComponentSpacePoseContext& Output)
{
	// Evaluate the input
	ComponentPose.EvaluateComponentSpace(Output);


	// If any Axis is checked, and both bones are present, then we will continue
	if (((bUpdateX) || (bUpdateY) || (bUpdateZ)) && IsValidToEvaluate(Output.AnimInstance->CurrentSkeleton, Output.AnimInstance->RequiredBones))
	{
		USkeletalMeshComponent* Component = Output.AnimInstance->GetSkelMeshComponent();
		TArray<FBoneTransform> BoneTransforms;
		// TODO Check why it doesn't work anymore.
		
		//EvaluateBoneTransforms(Component, Output.AnimInstance->RequiredBones, Output.Pose, BoneTransforms);

		checkSlow(!ContainsNaN(BoneTransforms));

		if (BoneTransforms.Num() > 0)
		{
			Output.Pose.LocalBlendCSBoneTransforms(BoneTransforms, BlendWeight);
		}
	}
}

//This Evaluates the bones, and transforms	
//void FAnimNode_ModifyFinger::EvaluateBoneTransforms(USkeletalMeshComponent* SkelComp, const FBoneContainer & RequiredBones, FCSPose<FCompactPose>& MeshBases, TArray<FBoneTransform>& OutBoneTransforms)
//{
//	check(OutBoneTransforms.Num() == 0);
//
//	// Component Space Transforms used to modify Target Bone
//	FTransform SourceBoneTM = MeshBases.GetComponentSpaceTransform(FCompactPoseBoneIndex(SourceBone.BoneIndex));
//	FTransform TargetBoneTM = MeshBases.GetComponentSpaceTransform(FCompactPoseBoneIndex(SourceBone.BoneIndex));
//
//	// If any changes have occured, add them, and reset offset
//	if (!(CurrentAddedOffset == AddtoOffset))
//	{
//		CurrentAddedOffset = SkelComp->ComponentToWorld.InverseTransformPosition(AddtoOffset);
//		JointOffset.Set((NULL), (NULL), (NULL));
//	}
//
//	if ((JointOffset.X == NULL) || (JointOffset.Y == NULL) || (JointOffset.Z == NULL))// If no Axis are checked, no need to do anything
//	{
//		// These are based on the Base Pose location and used to create the default offset
//		FVector tempSourceOffset;
//		FVector tempTargetOffset;
//
//		// Get the Base Pose of the Source Bone
//		tempSourceOffset.Set((SourceBoneTM.GetLocation().X), (SourceBoneTM.GetLocation().Y), (SourceBoneTM.GetLocation().Z));
//
//		// Get the Base Pose of the Target Bone
//		tempTargetOffset.Set((TargetBoneTM.GetLocation().X), (TargetBoneTM.GetLocation().Y), (TargetBoneTM.GetLocation().Z));
//
//		// Get the Starting Offset from ALL inputs
//		JointOffset.Set((tempTargetOffset.X - tempSourceOffset.X + CurrentAddedOffset.X), (tempTargetOffset.Y - tempSourceOffset.Y + CurrentAddedOffset.Y), (tempTargetOffset.Z - tempSourceOffset.Z + CurrentAddedOffset.Z));
//
//		// Assign Offset to Mod Target Location
//		ModTargetLocationInCompSpace.Set((tempTargetOffset.X), (tempTargetOffset.Y), (tempTargetOffset.Z));
//
//
//	}
//	//need to add a multiplier control so that you can control how much of the source movement is translated to the target(I.E. BlendEffects)
//	if (bUpdateX)//Update X if checked
//	{
//		ModTargetLocationInCompSpace.X = ((SourceBoneTM.GetTranslation().X) + JointOffset.X);//SourceTM + Offset
//	}
//	if (bUpdateY)//Update Y if checked
//	{
//		ModTargetLocationInCompSpace.Y = ((SourceBoneTM.GetTranslation().Y) + JointOffset.Y);
//	}
//	if (bUpdateZ)//Update Z if checked
//	{
//		ModTargetLocationInCompSpace.Z = ((SourceBoneTM.GetTranslation().Z) + JointOffset.Z); //(SourceBoneTM.GetTranslation().Z);
//	}
//	//Set the Targets Transform and pass out to Output(End of tick)
//	TargetBoneTM.SetTranslation(ModTargetLocationInCompSpace);
//
//	// Returns to caller with new location
//		// TODO Check why it doesn't work anymore.
//	 OutBoneTransforms.Add(FBoneTransform(FCompactPoseBoneIndex(TargetBone.BoneIndex), TargetBoneTM));
//}

// Makes sure that both bones are valid
bool FAnimNode_ModifyFinger::IsValidToEvaluate(const USkeleton * Skeleton, const FBoneContainer & RequiredBones)
{
	// if both bones are valid
	return (TargetBone.IsValid(RequiredBones) && (SourceBone.IsValid(RequiredBones)));
}*/