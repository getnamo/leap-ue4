// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "AnimHand.h"
#include "AnimFinger.h"
#include "DebugHandUtility.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAPMOTION_API UDebugHandUtility : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDebugHandUtility();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void DebugSphereAtActor(FVector Location, float Scale, float Radius, FVector Offset);
	void DebugFinger(UAnimFinger* AnimFinger, float Scale, FVector Offset);
	void DebugHand(UAnimHand* AnimHand, float Scale, FVector Offset);
	void DebugFullHand(UAnimHand* AnimHand, float Scale, FVector Offset);
	void DebugHandOrientation(UAnimHand* AnimHand);
	void DebugOrientation(FRotator Orientation, FVector At, float Scale);
	void DebugOrientationAtRelative(FRotator Orientation, FVector Relative, FVector Offset, float Scale);
	
};


