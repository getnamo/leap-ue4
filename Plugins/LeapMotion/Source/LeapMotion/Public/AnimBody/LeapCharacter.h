// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeapMotionPublicPCH.h"
#include "GameFramework/Character.h"
#include "LeapEventInterface.h"
#include "LeapController.h"
#include "LeapAnimBodyConnector.h"
#include "AnimBodyInterface.h"
#include "BasicRiggedAnim.h"
#include "LeapCharacter.generated.h"

UCLASS()
class LEAPMOTION_API ALeapCharacter : public ACharacter, public IAnimBodyInterface
{

public:

	virtual UAnimBody* GetAnimBody() override{
		if(LeapAnimBodyChildActor->ChildActor)
			return Cast<ALeapAnimBodyConnector>(LeapAnimBodyChildActor->ChildActor)->GetAnimBody();
		return nullptr;
	};
	virtual FString ToString() override{
		return TEXT("ALeapCharacter::ToString()");
	};

private:
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UAnimBlueprintGeneratedClass* AnimBlueprintToUse;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* LeapAnimBodyChildActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BodyInput", meta = (AllowPrivateAccess = "true"))
		class ALeapAnimBodyConnector* LeapAnimBodyActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float BaseTurnRate_0 = 45.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float BaseLookUpRate_0 = 45.0f;
	
	USceneComponent* Scene;

public:
	// Sets default values for this character's properties
	ALeapCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;	
};
