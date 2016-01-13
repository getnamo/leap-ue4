#pragma once

#include "AnimBone.h"
#include "LeapEnums.h"
#include "AnimFinger.generated.h"

UENUM(BlueprintType)
enum AnimHandType
{
	ANIM_HAND_UNKNOWN,
	ANIM_HAND_LEFT,
	ANIM_HAND_RIGHT
};

UCLASS(BlueprintType)
class LEAPMOTION_API UAnimFinger : public UObject
{
	GENERATED_UCLASS_BODY()

	//Not used in basic animation
	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	UAnimBone* Metacarpal;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	UAnimBone* Proximal;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	UAnimBone* Intermediate;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	UAnimBone* Distal;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	float Alpha;

	UFUNCTION(BlueprintCallable, Category = "Anim Finger")
	bool Enabled();

	UFUNCTION(BlueprintCallable, Category = "Anim Finger")
	void SetEnabled(bool enable = true);

	UFUNCTION(BlueprintCallable, Category = "Anim Finger")
	void TranslateFinger(FVector shift);

	UFUNCTION(BlueprintCallable, Category = "Anim Finger")
	void ChangeBasis(FRotator PreBase, FRotator PostBase, bool adjustVectors = true);

	UFUNCTION(BlueprintCallable, Category = "Anim Finger")
	void SetFromLeapFinger(class ULeapFinger* finger, LeapHandType handType);
};