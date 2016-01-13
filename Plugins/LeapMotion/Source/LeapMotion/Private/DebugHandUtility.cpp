// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapMotionPrivatePCH.h"
#include "DebugHandUtility.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDebugHandUtility::UDebugHandUtility()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDebugHandUtility::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDebugHandUtility::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UDebugHandUtility::DebugSphereAtActor(FVector Location, float Scale, float Radius, FVector Offset)
{
	FVector res = Scale*this->GetComponentRotation().RotateVector(Location + Offset) + this->GetComponentLocation();
	DrawDebugSphere(GetWorld(), GetComponentRotation().RotateVector(res), Radius, 12, FColor::Black);
}

void UDebugHandUtility::DebugFinger(UAnimFinger* AnimFinger, float Scale, FVector Offset)
{
	DebugSphereAtActor(AnimFinger->Distal->Position, Scale, Scale, Offset);
	DebugSphereAtActor(AnimFinger->Intermediate->Position, Scale, Scale, Offset);
	DebugSphereAtActor(AnimFinger->Proximal->Position, Scale, Scale, Offset);
	DebugSphereAtActor(AnimFinger->Metacarpal->Position, Scale, Scale, Offset);
}

void UDebugHandUtility::DebugHand(UAnimHand* AnimHand, float Scale, FVector Offset)
{
	DebugSphereAtActor(AnimHand->LowerArm->Position, Scale, Scale, Offset);
	DebugSphereAtActor(AnimHand->Wrist->Position, Scale, Scale, Offset);
	DebugSphereAtActor(AnimHand->Palm->Position, Scale, Scale, Offset);
}

void UDebugHandUtility::DebugFullHand(UAnimHand* AnimHand, float Scale, FVector Offset)
{
	DebugHand(AnimHand, Scale, Offset);
	DebugFinger(AnimHand->Thumb, Scale, Offset);
	DebugFinger(AnimHand->Index, Scale, Offset);
	DebugFinger(AnimHand->Middle, Scale, Offset);
	DebugFinger(AnimHand->Ring, Scale, Offset);
	DebugFinger(AnimHand->Pinky, Scale, Offset);
}

void UDebugHandUtility::DebugHandOrientation(UAnimHand* AnimHand)
{
	DebugOrientation(AnimHand->Wrist->Orientation, AnimHand->Wrist->Position + GetComponentLocation(), 50);
}

void UDebugHandUtility::DebugOrientation(FRotator Orientation, FVector At, float Scale)
{
	FRotator rotation = UKismetMathLibrary::ComposeRotators(Orientation, GetComponentRotation());
	FVector EndArrow = (UKismetMathLibrary::GetForwardVector(rotation)*Scale) + At; 
	DrawDebugDirectionalArrow(GetWorld(), At, EndArrow, 5, FColor::Black); 

	FVector Extent = Scale * FVector(0.2, 0.2, 0.2);
	DrawDebugBox(GetWorld(), EndArrow, Extent, rotation.Quaternion(), FColor::Black);
}

void UDebugHandUtility::DebugOrientationAtRelative(FRotator Orientation, FVector Relative, FVector Offset, float Scale)
{
	FVector position =  GetComponentRotation().RotateVector(Relative + Offset) + GetComponentLocation();
	DebugOrientation(Orientation, position, Scale);
}