// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapMotionPrivatePCH.h"
#include "LeapEchoCharacter.h"


// Sets default values
ALeapEchoCharacter::ALeapEchoCharacter()
	:ALeapCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> Material(TEXT("Material'/LeapMotion/Character/Echo/hand_mat.hand_mat'"));
	GetMesh()->SetMaterial(0, Material.Object);

	// Set Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/LeapMotion/Character/Echo/hands_lo.hands_lo'"));
	GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
	GetMesh()->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
	GetMesh()->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("AnimBlueprint'/LeapMotion/Character/Echo/LeapEchoHands_AnimBlueprint2.LeapEchoHands_AnimBlueprint2'"));
	GetMesh()->AnimBlueprintGeneratedClass = AnimBlueprint.Object->GetAnimBlueprintGeneratedClass();
}