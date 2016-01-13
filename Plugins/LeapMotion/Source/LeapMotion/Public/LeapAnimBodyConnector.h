// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LeapEventInterface.h"
#include "LeapController.h"
#include "AnimBody.h"
#include "DebugHandUtility.h"
#include "AnimBodyInterface.h"
#include "LeapAnimBodyConnector.generated.h"

UCLASS()
class LEAPMOTION_API ALeapAnimBodyConnector : public AActor, public ILeapEventInterface, public IAnimBodyInterface
{
public:
	virtual UAnimBody* GetAnimBody() override
	{
		return AnimBody;
	};
	virtual FString ToString() override;
private:
	GENERATED_BODY()

	// Variables 
	bool HMDStateChanged;
	FVector LeapBaseOffset;
	FVector OwnerToLeap;
	FVector OwnerMeshToRoot;
	FVector LeapToComponentShift;
	float Alpha;
	bool HMDEnabled;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rigging", meta = (AllowPrivateAccess = "true"))
		bool IsFacingModeHMD;

	// Should rig or not?
		bool RiggingAdjustmentEnabled = true;

	// Active debug preview
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
		bool DebugPreview = true;

	// Basis
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basis", meta = (AllowPrivateAccess = "true"))
		FRotator LeftPreBase = FRotator(0.0f, 0.0f, -90.0f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basis", meta = (AllowPrivateAccess = "true"))
		FRotator LeftPostBase = FRotator(0.0f, 90.0f, 0.0f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basis", meta = (AllowPrivateAccess = "true"))
		FRotator RightPreBase = FRotator(0.0f, 180.0f, 90.0f);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Basis", meta = (AllowPrivateAccess = "true"))
		FRotator RightPostBase = FRotator(0.0f, 90.0f, 0.0f);

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class ULeapController* LeapController;
	UPROPERTY(VisibleAnywhere)
		class UAnimBody* AnimBody;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* Arrow;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBillboardComponent* DefaultModeSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBillboardComponent* HMDModeSprite;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UDebugHandUtility* DebugHand;

	// Methods
	UObject* GetOwnerComponentClass(UClass* Class) const;
	void SetOwnerLinks();
	void HandleHMDModeChanged();
	void DisplayDebugHandIfEnabled(UAnimHand* Hand);
	void SetAnimHandFromLeapHand(UAnimHand* AnimHand, ULeapHand* LeapHand, FRotator PreBase, FRotator PostBase);

	
public:	
	// Sets default values for this actor's properties
	ALeapAnimBodyConnector();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool HasHMDModeChanged();
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void OnHandCountChanged(int32 count) override;
	void OnLeapHandMoved(ULeapHand* hand) override;
	
};
