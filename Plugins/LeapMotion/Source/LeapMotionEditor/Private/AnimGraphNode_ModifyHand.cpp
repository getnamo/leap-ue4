// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "LeapMotionEditorPCH.h"
//#include "Editor/AnimGraph/Private/AnimGraphPrivatePCH.h"
//#include "AnimationGraphSchema.h"
#include "AnimGraphNode_ModifyHand.h"
#include "CompilerResultsLog.h"

/////////////////////////////////////////////////////
// UAnimGraphNode_ModifyHand


#define LOCTEXT_NAMESPACE "A3Nodes"

UAnimGraphNode_ModifyHand::UAnimGraphNode_ModifyHand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurWidgetMode = (int32)FWidget::WM_Rotate;
}

void UAnimGraphNode_ModifyHand::ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog)
{
	Node.InitHandBonesReferences();
	for (int32 index = 0; index < 15; index++)
	{
		ValidateAnimNodeDuringCompilationOneBone(ForSkeleton, Node.HandBonesReferences->All[index], MessageLog);
	}
	Super::ValidateAnimNodeDuringCompilation(ForSkeleton, MessageLog);
}

void UAnimGraphNode_ModifyHand::ValidateAnimNodeDuringCompilationOneBone(USkeleton* ForSkeleton, FBoneReference* BoneToValidate, FCompilerResultsLog &MessageLog)
{
	if (ForSkeleton->GetReferenceSkeleton().FindBoneIndex(BoneToValidate->BoneName) == INDEX_NONE)
	{
		MessageLog.Warning(*LOCTEXT("NoBoneToModify", "@@ - You must pick a bone to modify").ToString(), this);
	}
	/*
	if (Node.RotationMode == BMM_Ignore)
	{
		MessageLog.Warning(*LOCTEXT("NothingToModify", "@@ - No components to modify selected.  Either Rotation, Translation, or Scale should be set to something other than Ignore").ToString(), this);
	}*/
}

FText UAnimGraphNode_ModifyHand::GetControllerDescription() const
{
	return LOCTEXT("TransformModifyHand", "Transform (Modify) Hand");
}

FText UAnimGraphNode_ModifyHand::GetTooltipText() const
{
	return LOCTEXT("AnimGraphNode_ModifyHand_Tooltip", "The Transform Bone node alters the transform - i.e. Translation, Rotation, or Scale - of the bone");
}

FText UAnimGraphNode_ModifyHand::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	
	//if ((TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle) && (Node.BoneToModify.BoneName == NAME_None))
	{
		return GetControllerDescription();
	}
	// @TODO: the bone can be altered in the property editor, so we have to 
	//        choose to mark this dirty when that happens for this to properly work
	/*else //if (!CachedNodeTitles.IsTitleCached(TitleType, this))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ControllerDescription"), GetControllerDescription());
		Args.Add(TEXT("BoneName"), FText::FromName(Node.BoneToModify.BoneName));

		// FText::Format() is slow, so we cache this to save on performance
		if (TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle)
		{
			CachedNodeTitles.SetCachedTitle(TitleType, FText::Format(LOCTEXT("AnimGraphNode_ModifyBone_ListTitle", "{ControllerDescription} - Bone: {BoneName}"), Args), this);
		}
		else
		{
			CachedNodeTitles.SetCachedTitle(TitleType, FText::Format(LOCTEXT("AnimGraphNode_ModifyBone_Title", "{ControllerDescription}\nBone: {BoneName}"), Args), this);
		}
	}
	return CachedNodeTitles[TitleType];
	*/
}

int32 UAnimGraphNode_ModifyHand::GetWidgetCoordinateSystem(const USkeletalMeshComponent* SkelComp)
{
	EBoneControlSpace Space = BCS_BoneSpace;
	switch (CurWidgetMode)
	{
	case FWidget::WM_Rotate:
		Space = Node.RotationSpace;
		break;
	case FWidget::WM_Translate:
		Space = Node.TranslationSpace;
		break;
	case FWidget::WM_Scale:
		Space = Node.ScaleSpace;
		break;
	}

	switch (Space)
	{
	default:
	case BCS_ParentBoneSpace:
		//@TODO: No good way of handling this one
		return COORD_World;
	case BCS_BoneSpace:
		return COORD_Local;
	case BCS_ComponentSpace:
	case BCS_WorldSpace:
		return COORD_World;
	}
}

FVector UAnimGraphNode_ModifyHand::GetWidgetLocation(const USkeletalMeshComponent* SkelComp, FAnimNode_SkeletalControlBase* AnimNode)
{
	USkeleton* Skeleton = SkelComp->SkeletalMesh->Skeleton;
	FVector WidgetLoc = FVector::ZeroVector;
	/*
	// if the current widget mode is translate, then shows the widget according to translation space
	if (CurWidgetMode == FWidget::WM_Translate)
	{
		FCSPose<FCompactPose>& MeshBases = AnimNode->ForwardedPose;
		WidgetLoc = ConvertWidgetLocation(SkelComp, MeshBases, Node.TargetFingerBone1.BoneName, GetNodeValue(FString("Translation"), Node.Translation1), Node.TranslationSpace);

		if (MeshBases.GetPose().IsValid() && Node.TranslationMode == BMM_Additive)
		{
			if (Node.TranslationSpace == EBoneControlSpace::BCS_WorldSpace ||
				Node.TranslationSpace == EBoneControlSpace::BCS_ComponentSpace)
			{
				const FMeshPoseBoneIndex MeshBoneIndex(SkelComp->GetBoneIndex(Node.TargetFingerBone1.BoneName));
				const FCompactPoseBoneIndex BoneIndex = MeshBases.GetPose().GetBoneContainer().MakeCompactPoseIndex(MeshBoneIndex);

				if (BoneIndex != INDEX_NONE)
				{
					const FTransform& BoneTM = MeshBases.GetComponentSpaceTransform(BoneIndex);
					WidgetLoc += BoneTM.GetLocation();
				}
			}
		}
	}
	else // if the current widget mode is not translate mode, then show the widget on the bone to modify
	{
		int32 MeshBoneIndex = SkelComp->GetBoneIndex(Node.TargetFingerBone1.BoneName);

		if (MeshBoneIndex != INDEX_NONE)
		{
			const FTransform BoneTM = SkelComp->GetBoneTransform(MeshBoneIndex);
			WidgetLoc = BoneTM.GetLocation();
		}
	}
	*/
	return WidgetLoc;
}

EBoneModificationMode UAnimGraphNode_ModifyHand::GetBoneModificationMode(int32 InWidgetMode)
{
	FWidget::EWidgetMode InMode = (FWidget::EWidgetMode)InWidgetMode;
	switch (InMode)
	{
	case FWidget::WM_Translate:
		return Node.TranslationMode;
		break;
	case FWidget::WM_Rotate:
		return Node.RotationMode;
		break;
	case FWidget::WM_Scale:
		return Node.ScaleMode;
		break;
	case FWidget::WM_TranslateRotateZ:
	case FWidget::WM_2D:
		break;
	}

	return EBoneModificationMode::BMM_Ignore;
}

int32 UAnimGraphNode_ModifyHand::GetNextWidgetMode(int32 InWidgetMode)
{
	FWidget::EWidgetMode InMode = (FWidget::EWidgetMode)InWidgetMode;
	switch (InMode)
	{
	case FWidget::WM_Translate:
		return FWidget::WM_Rotate;
	case FWidget::WM_Rotate:
		return FWidget::WM_Scale;
	case FWidget::WM_Scale:
		return FWidget::WM_Translate;
	case FWidget::WM_TranslateRotateZ:
	case FWidget::WM_2D:
		break;
	}

	return (int32)FWidget::WM_None;
}

int32 UAnimGraphNode_ModifyHand::FindValidWidgetMode(int32 InWidgetMode)
{
	FWidget::EWidgetMode InMode = (FWidget::EWidgetMode)InWidgetMode;
	FWidget::EWidgetMode ValidMode = InMode;
	if (InMode == FWidget::WM_None)
	{	// starts from Rotate mode
		ValidMode = FWidget::WM_Rotate;
	}

	// find from current widget mode and loop 1 cycle until finding a valid mode
	for (int32 Index = 0; Index < 3; Index++)
	{
		if (GetBoneModificationMode(ValidMode) != BMM_Ignore)
		{
			return ValidMode;
		}

		ValidMode = (FWidget::EWidgetMode)GetNextWidgetMode(ValidMode);
	}

	// if couldn't find a valid mode, returns None
	ValidMode = FWidget::WM_None;

	return (int32)ValidMode;
}

int32 UAnimGraphNode_ModifyHand::GetWidgetMode(const USkeletalMeshComponent* SkelComp)
{
	int32 BoneIndex = SkelComp->GetBoneIndex(Node.BoneIndex1.BoneName);
	if (BoneIndex != INDEX_NONE)
	{
		CurWidgetMode = FindValidWidgetMode(CurWidgetMode);
		return CurWidgetMode;
	}
	
	return (int32)FWidget::WM_None;
}

int32 UAnimGraphNode_ModifyHand::ChangeToNextWidgetMode(const USkeletalMeshComponent* SkelComp, int32 InCurWidgetMode)
{
	int32 NextWidgetMode = GetNextWidgetMode(InCurWidgetMode);
	CurWidgetMode = FindValidWidgetMode(NextWidgetMode);

	return CurWidgetMode;
}

bool UAnimGraphNode_ModifyHand::SetWidgetMode(const USkeletalMeshComponent* SkelComp, int32 InWidgetMode)
{
	// if InWidgetMode is available 
	if (FindValidWidgetMode(InWidgetMode) == InWidgetMode)
	{
		CurWidgetMode = InWidgetMode;
		return true;
	}

	return false;
}

FName UAnimGraphNode_ModifyHand::FindSelectedBone()
{
	return Node.BoneIndex1.BoneName;
}

void UAnimGraphNode_ModifyHand::DoTranslation(const USkeletalMeshComponent* SkelComp, FVector& Drag, FAnimNode_Base* InOutAnimNode)
{
	/*FAnimNode_ModifyHand* ModifyBone = static_cast<FAnimNode_ModifyHand*>(InOutAnimNode);
	
	if (ModifyBone->TranslationMode != EBoneModificationMode::BMM_Ignore)
	{
		FVector Offset = ConvertCSVectorToBoneSpace(SkelComp, Drag, ModifyBone->ForwardedPose, Node.TargetFingerBone1.BoneName, Node.TranslationSpace);
		//ModifyBone->Translation1 += Offset;

		Offset = ConvertCSVectorToBoneSpace(SkelComp, Drag, ModifyBone->ForwardedPose, Node.TargetFingerBone2.BoneName, Node.TranslationSpace);
		//ModifyBone->Translation2 += Offset;

		Offset = ConvertCSVectorToBoneSpace(SkelComp, Drag, ModifyBone->ForwardedPose, Node.TargetFingerBone3.BoneName, Node.TranslationSpace);
		//ModifyBone->Translation3 += Offset;

		//Node.Translation1 = ModifyBone->Translation1;
		//Node.Translation2 = ModifyBone->Translation2;
		//Node.Translation3 = ModifyBone->Translation3;
	}*/
}

void UAnimGraphNode_ModifyHand::DoRotation(const USkeletalMeshComponent* SkelComp, FRotator& Rotation, FAnimNode_Base* InOutAnimNode)
{/*
	FAnimNode_ModifyHand* ModifyBone = static_cast<FAnimNode_ModifyHand*>(InOutAnimNode);

	if (Node.RotationMode != EBoneModificationMode::BMM_Ignore)
	{
		//DoRotationOneBone(SkelComp, Rotation, ModifyBone, &ModifyBone->Rotation1, &Node.TargetFingerBone1, &Node.Rotation1);
		//DoRotationOneBone(SkelComp, Rotation, ModifyBone, &ModifyBone->Rotation2, &Node.TargetFingerBone2, &Node.Rotation2);
		//DoRotationOneBone(SkelComp, Rotation, ModifyBone, &ModifyBone->Rotation3, &Node.TargetFingerBone3, &Node.Rotation3);
	}*/

}

void UAnimGraphNode_ModifyHand::DoRotationOneBone(const USkeletalMeshComponent* SkelComp, FRotator& Rotation, FAnimNode_ModifyHand* ModifyNode, FRotator* ModifyNodeBoneRotation, FBoneReference* CurrentNodeBone, FRotator* CurrentNodeBoneRotation)
{
	FQuat DeltaQuat = ConvertCSRotationToBoneSpace(SkelComp, Rotation, ModifyNode->ForwardedPose, CurrentNodeBone->BoneName, Node.RotationSpace);
	FQuat PrevQuat(*ModifyNodeBoneRotation);
	FQuat NewQuat = DeltaQuat * PrevQuat;
	*ModifyNodeBoneRotation = NewQuat.Rotator();
	*CurrentNodeBoneRotation = NewQuat.Rotator();
}

void UAnimGraphNode_ModifyHand::DoScale(const USkeletalMeshComponent* SkelComp, FVector& Scale, FAnimNode_Base* InOutAnimNode)
{
	FAnimNode_ModifyBone* ModifyBone = static_cast<FAnimNode_ModifyBone*>(InOutAnimNode);
	if (Node.ScaleMode != EBoneModificationMode::BMM_Ignore)
	{
		FVector Offset = Scale;
		//ModifyBone->Scale += Offset;
		//Node.Scale = ModifyBone->Scale;
	}
}

void UAnimGraphNode_ModifyHand::CopyNodeDataTo(FAnimNode_Base* OutAnimNode)
{
	/*FAnimNode_ModifyHand* ModifyBone = static_cast<FAnimNode_ModifyHand*>(OutAnimNode);
	
	// copies Pin values from the internal node to get data which are not compiled yet
	ModifyBone->Translation1 = GetNodeValue(FString("Translation"), Node.Translation1);
	ModifyBone->Translation2 = GetNodeValue(FString("Translation"), Node.Translation2);
	ModifyBone->Translation3 = GetNodeValue(FString("Translation"), Node.Translation3);
	ModifyBone->Rotation1 = GetNodeValue(FString("Rotation1"), Node.Rotation1);
	ModifyBone->Rotation2 = GetNodeValue(FString("Rotation2"), Node.Rotation2);
	ModifyBone->Rotation3 = GetNodeValue(FString("Rotation3"), Node.Rotation3);
	ModifyBone->Scale = GetNodeValue(FString("Scale"), Node.Scale);

	// copies Modes
	ModifyBone->TranslationMode = Node.TranslationMode;
	ModifyBone->RotationMode = Node.RotationMode;
	ModifyBone->ScaleMode = Node.ScaleMode;

	// copies Spaces
	ModifyBone->TranslationSpace = Node.TranslationSpace;
	ModifyBone->RotationSpace = Node.RotationSpace;
	ModifyBone->ScaleSpace = Node.ScaleSpace;*/
	
}

void UAnimGraphNode_ModifyHand::CopyNodeDataFrom(const FAnimNode_Base* InNewAnimNode)
{
	/*const FAnimNode_ModifyHand* ModifyBone = static_cast<const FAnimNode_ModifyHand*>(InNewAnimNode);
	
	switch (CurWidgetMode)
	{
	case FWidget::WM_Translate:
		SetNodeValue(FString("Translation1"), Node.Translation1, ModifyBone->Translation1);
		SetNodeValue(FString("Translation2"), Node.Translation2, ModifyBone->Translation2);
		SetNodeValue(FString("Translation3"), Node.Translation3, ModifyBone->Translation3);
		break;
	case FWidget::WM_Rotate:
		SetNodeValue(FString("Rotation1"), Node.Rotation1, ModifyBone->Rotation1);
		SetNodeValue(FString("Rotation2"), Node.Rotation2, ModifyBone->Rotation2);
		SetNodeValue(FString("Rotation3"), Node.Rotation3, ModifyBone->Rotation3);
		break;
	case FWidget::WM_Scale:
		SetNodeValue(FString("Scale"), Node.Scale, ModifyBone->Scale);
		break;
	}*/
}
#undef LOCTEXT_NAMESPACE
