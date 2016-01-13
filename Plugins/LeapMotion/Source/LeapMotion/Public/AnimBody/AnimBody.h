#pragma once

#include "AnimHand.h"
#include "AnimBody.generated.h"

//NB: this is a limited class used only for leap anim, full class will have full body

UCLASS(ClassGroup = "Animation Skeleton", meta = (BlueprintSpawnableComponent))
class LEAPMOTION_API UAnimBody : public UActorComponent
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Anim Body")
	float Alpha;

	//Hands
	UPROPERTY(VisibleAnywhere, Category = "Anim Body")
	UAnimHand* Left;

	UPROPERTY(VisibleAnywhere, Category = "Anim Body")
	UAnimHand* Right;

	//Head
	UPROPERTY(VisibleAnywhere, Category = "Anim Body")
	UAnimBone* Head;


	UFUNCTION(BlueprintCallable, Category = "Anim Body")
	bool Enabled();

	UFUNCTION(BlueprintCallable, Category = "Anim Body")
	void SetEnabled(bool enable = true);

	UFUNCTION(BlueprintCallable, Category = "Anim Body")
	void TranslateBody(FVector shift);

	UFUNCTION(BlueprintCallable, Category = "Anim Body")
	void ChangeBasis(FRotator PreBase, FRotator PostBase, bool adjustVectors = true);
};