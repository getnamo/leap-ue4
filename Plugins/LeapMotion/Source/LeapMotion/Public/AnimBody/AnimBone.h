#pragma once

#include "Core.h"
#include "AnimBone.generated.h"

UCLASS(BlueprintType)
class LEAPMOTION_API UAnimBone : public UObject
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	FVector Position;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	FRotator Orientation;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	FVector Scale;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	float Length;

	UPROPERTY(VisibleAnywhere, Category = "Anim Bone")
	float Alpha;


	UFUNCTION(BlueprintCallable, Category = "Anim Bone")
	bool Enabled();

	UFUNCTION(BlueprintCallable, Category = "Anim Bone")
	void SetEnabled(bool enable = true);

	UFUNCTION(BlueprintCallable, Category = "Anim Bone")
	void SetFromTransform(const FTransform& in);

	UFUNCTION(BlueprintCallable, Category = "Anim Bone")
	FTransform GetTransform();

	UFUNCTION(BlueprintCallable, Category = "Anim Bone")
	void TranslateBone(FVector shift);

	UFUNCTION(BlueprintCallable, Category = "Anim Bone")
	void ChangeBasis(FRotator PreBase, FRotator PostBase, bool adjustVectors = true);
};