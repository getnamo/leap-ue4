// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapMotionPrivatePCH.h"
#include "LeapCharacter.h"
#include "LeapAnimBodyConnector.h"


// Sets default values
ALeapCharacter::ALeapCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(Scene);

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> Material(TEXT("MaterialInstanceConstant'/LeapMotion/Materials/MI_Template_BaseYellow.MI_Template_BaseYellow'"));
	GetMesh()->SetMaterial(0, Material.Object);

	// Set Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->RelativeLocation = FVector(10.0f, 0.0f, 95.0f);
	Camera->bUsePawnControlRotation = 1.0f, 
	Camera->AttachTo(GetCapsuleComponent());

	// Set AnimBodyConnector
	LeapAnimBodyChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("LeapAnimBodyChildActorComponent"));
	LeapAnimBodyChildActor->RelativeLocation = FVector(10.0f, 0.0f, 95.0f);
	LeapAnimBodyChildActor->SetChildActorClass(ALeapAnimBodyConnector::StaticClass());
	LeapAnimBodyChildActor->AttachTo(GetCapsuleComponent());
	// Saving Ref to connector
	LeapAnimBodyActor = Cast<ALeapAnimBodyConnector>(LeapAnimBodyChildActor->ChildActor);
	
	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/LeapMotion/Character/Basic/BasicBody.BasicBody'"));
	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
	GetMesh()->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("AnimBlueprint'/LeapMotion/Character/Basic/BasicCharacter_AnimBlueprint2.BasicCharacter_AnimBlueprint2'"));
	GetMesh()->AnimBlueprintGeneratedClass = AnimBlueprint.Object->GetAnimBlueprintGeneratedClass();
}

// Called when the game starts or when spawned
void ALeapCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALeapCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ALeapCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

