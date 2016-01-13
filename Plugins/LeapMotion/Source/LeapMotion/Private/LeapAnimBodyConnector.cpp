// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapMotionPrivatePCH.h"
#include "LeapAnimBodyConnector.h"
#include "HeadMountedDisplay.h"


FString ALeapAnimBodyConnector::ToString()
{
	return "ALeapAnimBodyConnector::ToString()";
}


UObject* ALeapAnimBodyConnector::GetOwnerComponentClass(UClass* Class) const
{
	//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector GetOwnerComponentClass!"));
	// return the first of the array
	if(RootComponent != nullptr 
		&& RootComponent->GetAttachParent() != nullptr 
		&& RootComponent->GetAttachParent()->GetOwner() != nullptr)
	{
		return RootComponent->GetAttachParent()->GetOwner()->GetComponentsByClass(Class)[0];
	}
	return nullptr;
}

void ALeapAnimBodyConnector::SetOwnerLinks()
{
	//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector SetOwnerLinks!"));
	UObject* ChildActorAsObject = GetOwnerComponentClass(UChildActorComponent::StaticClass());
	if(ChildActorAsObject != nullptr)
	{
		UChildActorComponent* ChildActor = dynamic_cast<UChildActorComponent*>(ChildActorAsObject);
		OwnerToLeap = ChildActor->RelativeLocation;
	}
	else
	{
		OwnerToLeap = FVector(0.0f, 0.0f, 0.0f);
	}

	UObject* SkeletalMeshAsObject = GetOwnerComponentClass(USkeletalMeshComponent::StaticClass());
	if (SkeletalMeshAsObject != nullptr)
	{
		USkeletalMeshComponent* SkeletalMesh = dynamic_cast<USkeletalMeshComponent*>(SkeletalMeshAsObject);
		OwnerMeshToRoot = (-1) * SkeletalMesh->RelativeLocation;
	}
	else
	{
		OwnerMeshToRoot = FVector(0.0f, 0.0f, 0.0f);
	}

	HandleHMDModeChanged();
}

void ALeapAnimBodyConnector::HandleHMDModeChanged()
{
	//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector HandleHMDModeChanged!"));
	FString hmd = FString(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() ? TEXT("true") : TEXT("false"));
	FString FacingMode = FString(IsFacingModeHMD ? TEXT("true") : TEXT("false"));
	//UE_LOG(FantasiaInit, Log, TEXT("IsHeadMountedDisplayEnabled %s"), *hmd);
	//UE_LOG(FantasiaInit, Log, TEXT("IsFacingModeHMD %s"), *FacingMode);
	// Ensure we enable our anim body head when the state changes
	AnimBody->Head->SetEnabled(IsFacingModeHMD);
	LeapController->OptimizeForHMD(IsFacingModeHMD, true, true);
	
	// Set our component offset based on the mode
	LeapBaseOffset = IsFacingModeHMD ? HMDModeSprite->RelativeLocation : DefaultModeSprite->RelativeLocation;

	// Set the shift from leap to component
	LeapToComponentShift = LeapBaseOffset + OwnerToLeap + OwnerMeshToRoot;
}

void ALeapAnimBodyConnector::DisplayDebugHandIfEnabled(UAnimHand* Hand)
{
	//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector DisplayDebugHandIfEnabled!"));
	if(DebugPreview)
	{
		DebugHand->DebugFullHand(Hand, 1.0f, LeapBaseOffset);
		FVector extent(5.0f, 5.0f, 5.0f);
		DrawDebugBox(GetWorld(), DefaultModeSprite->GetComponentLocation(), extent, FColor::Green);
	}
}

void ALeapAnimBodyConnector::SetAnimHandFromLeapHand(UAnimHand* AnimHand, ULeapHand* LeapHand, FRotator PreBase, FRotator PostBase)
{
	//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector SetAnimHandFromLeapHand!"));
	AnimHand->SetFromLeapHand(LeapHand);
	DisplayDebugHandIfEnabled(AnimHand);
	AnimHand->TranslateHand(LeapToComponentShift);
	
	if(RiggingAdjustmentEnabled)
	{
		AnimHand->ChangeBasis(PreBase, PostBase, true);
	}

}

void ALeapAnimBodyConnector::OnHandCountChanged(int32 count)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LEAP HandCountChanged!"));
	//UE_LOG(FantasiaInit, Log, TEXT("LEAP HandCountChanged! %f"), count);
	ULeapHandList* Hands;
	// Both hands visible
	switch (count)
	{
		// Both hands visible
	case 2:
		AnimBody->Left->SetEnabled(true);
		AnimBody->Right->SetEnabled(true);
		break;

		// 1 hand visible
	case 1:
		Hands = LeapController->Frame(0)->Hands();
		for (int i = 0; i < Hands->Count; i++)
		{
			ULeapHand* Hand = Hands->getIndex(i);
			if (Hand->HandType == HAND_LEFT)
			{
				AnimBody->Left->SetEnabled(true);
				AnimBody->Right->SetEnabled(false);
				Alpha = 1.0f;
			}
			else if (Hand->HandType == HAND_RIGHT)
			{
				AnimBody->Left->SetEnabled(false);
				AnimBody->Right->SetEnabled(true);
				Alpha = 1.0f;
			}
		}
		
		break;

		// No hands visible
	case 0:
		AnimBody->Left->SetEnabled(false);
		AnimBody->Right->SetEnabled(false);
		break;

	default:
		break;
	}
}

void ALeapAnimBodyConnector::OnLeapHandMoved(ULeapHand* hand)
{
	//UE_LOG(FantasiaInit, Log, TEXT("LEAP LeapHandMoved!"));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LEAP LeapHandMoved!"));
	if (hand->HandType == HAND_LEFT)
	{
		SetAnimHandFromLeapHand(AnimBody->Left, hand, LeftPreBase, LeftPostBase);
	}
	else if (hand->HandType == HAND_RIGHT)
	{
		SetAnimHandFromLeapHand(AnimBody->Right, hand, RightPreBase, RightPostBase);		
	}
}


// Sets default values
ALeapAnimBodyConnector::ALeapAnimBodyConnector()
{
	//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector ALeapAnimBodyConnector!"));
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	RootComponent = Arrow;

	AnimBody = CreateDefaultSubobject<UAnimBody>(TEXT("AnimBodyComponent"));
	AnimBody->SetEnabled(true);

	LeapController = CreateDefaultSubobject<ULeapController>(TEXT("LeapControllerComponent"));
	LeapController->Activate();

	DebugHand = CreateDefaultSubobject<UDebugHandUtility>(TEXT("DebugHandComponent"));
	DebugHand->AttachTo(Arrow);

	static ConstructorHelpers::FObjectFinder<UTexture2D> DefaultTexture(TEXT("Texture2D'/LeapMotion/Materials/Icon128.Icon128'"));
	DefaultModeSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("DefaultModeSpriteComponent"));
	DefaultModeSprite->SetSprite(DefaultTexture.Object);
	DefaultModeSprite->RelativeLocation = FVector(50.0f, 0.0f, -50.0f);
	DefaultModeSprite->AttachTo(Arrow);

	HMDModeSprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("HMDModeSpriteComponent"));
	HMDModeSprite->SetSprite(DefaultTexture.Object);
	HMDModeSprite->AttachTo(Arrow);

	// Events
}

// Called when the game starts or when spawned
void ALeapAnimBodyConnector::BeginPlay()
{
	Super::BeginPlay();
	SetOwnerLinks();
}

bool ALeapAnimBodyConnector::HasHMDModeChanged()
{
	if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() != IsFacingModeHMD)
	{
		FString hmd = FString(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() ? TEXT("true") : TEXT("false"));
		FString FacingMode = FString(IsFacingModeHMD ? TEXT("true") : TEXT("false"));
		//UE_LOG(FantasiaInit, Log, TEXT("ALeapAnimBodyConnector HasHMDModeChanged"));
		//UE_LOG(FantasiaInit, Log, TEXT("IsHeadMountedDisplayEnabled %s"), *hmd);
		//UE_LOG(FantasiaInit, Log, TEXT("IsFacingModeHMD %s"), *FacingMode);
			
		IsFacingModeHMD = UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
		return true;
	}
	return false;
}

// Called every frame
void ALeapAnimBodyConnector::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//LeapController->ReceiveTick(DeltaTime);

	if(HMDEnabled)
	{
		FRotator Rotation;
		FVector Position;

		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Rotation, Position);
		Position += OwnerToLeap + OwnerMeshToRoot;

		AnimBody->Head->Position = Position;
		AnimBody->Head->Orientation = Rotation;
		AnimBody->ChangeBasis(FRotator(90, 90, 0), FRotator(0, 0, 90), true);
	}

	if(HasHMDModeChanged())
	{
		HandleHMDModeChanged();
	}
}

