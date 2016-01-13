#pragma once

#include "AnimFinger.h"
#include "LeapHand.h"
#include "AnimHand.generated.h"

UCLASS(BlueprintType)
class LEAPMOTION_API UAnimHand : public UObject
{
	GENERATED_UCLASS_BODY()
	//Hand Properties
	UPROPERTY(VisibleAnywhere, Category = "Anim Hand Finger")
	float Alpha;

	//Fingers
	UPROPERTY(VisibleAnywhere, Category = "Anim Hand Finger")
	UAnimFinger* Thumb;

	UPROPERTY(VisibleAnywhere, Category = "Anim Hand Finger")
	UAnimFinger* Index;

	UPROPERTY(VisibleAnywhere, Category = "Anim Hand Finger")
	UAnimFinger* Middle;

	UPROPERTY(VisibleAnywhere, Category = "Anim Hand Finger")
	UAnimFinger* Ring;

	UPROPERTY(VisibleAnywhere, Category = "Anim Hand Finger")
	UAnimFinger* Pinky;
	
	//Arm and Wrist, NB: Consider moving this up the skeletal tree, for leap this is sufficient
	UPROPERTY(BlueprintReadOnly, Category = "Anim Hand")
	UAnimBone* Wrist;

	UPROPERTY(BlueprintReadOnly, Category = "Anim Hand")
	UAnimBone* Palm;

	UPROPERTY(BlueprintReadOnly, Category = "Anim Hand")
	UAnimBone* LowerArm;

	UFUNCTION(BlueprintCallable, Category = "Anim Hand")
	bool Enabled();

	UFUNCTION(BlueprintCallable, Category = "Anim Hand")
	void SetEnabled(bool enable = true);

	UFUNCTION(BlueprintCallable, Category = "Anim Hand")
	void TranslateHand(FVector shift);

	UFUNCTION(BlueprintCallable, Category = "Anim Hand")
	void ChangeBasis(FRotator PreBase, FRotator PostBase, bool adjustVectors = true);


	//These will turn into interface events for the general case later on
	UFUNCTION(BlueprintCallable, Category = "Anim Hand")
	void SetFromLeapHand(class ULeapHand* leapHand);
};