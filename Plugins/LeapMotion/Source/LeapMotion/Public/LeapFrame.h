#pragma once

#include "LeapMotionPublicPCH.h"
#include "LeapFrame.generated.h"

UCLASS(BlueprintType)
class LEAPMOTION_API ULeapFrame : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	~ULeapFrame();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Frame")
	float CurrentFPS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Frame")
	bool IsValid;

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	class ULeapFinger* Finger(int32 id);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "fingers", CompactNodeTitle = "", Keywords = "get fingers"), Category = "Leap Frame")
	class ULeapFingerList* Fingers();

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	class ULeapGesture* Gesture(int32 id);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "gestures", CompactNodeTitle = "", Keywords = "get gestures"), Category = "Leap Frame")
	class ULeapGestureList* Gestures();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "gestures", CompactNodeTitle = "", Keywords = "get gestures"), Category = "Leap Frame")
	class ULeapGestureList* GesturesSinceFrame(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	class ULeapHand* Hand(int32 id);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getHands", CompactNodeTitle = "", Keywords = "get hands"), Category = "Leap Frame")
	class ULeapHandList* Hands();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "images", CompactNodeTitle = "", Keywords = "get images"), Category = "Leap Frame")
	class ULeapImageList* Images();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "interactionBox", CompactNodeTitle = "", Keywords = "get interaction box"), Category = "Leap Frame")
	class ULeapInteractionBox* InteractionBox();

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	class ULeapPointable* Pointable(int32 id);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "pointables", CompactNodeTitle = "", Keywords = "get pointables"), Category = "Leap Frame")
	class ULeapPointableList* Pointables();

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	float RotationAngle(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	float RotationAngleAroundAxis(class ULeapFrame* frame, FVector axis);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	FVector RotationAxis(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	float RotationProbability(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	float ScaleFactor(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	float ScaleProbability(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	class ULeapTool* Tool(int32 id);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "pointables", CompactNodeTitle = "", Keywords = "get pointables"), Category = "Leap Frame")
	class ULeapToolList* Tools();

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	FVector Translation(class ULeapFrame* frame);

	UFUNCTION(BlueprintCallable, Category = "Leap Frame")
	float TranslationProbability(class ULeapFrame* frame);

	void setFrame(Leap::Controller &leap, int history = 0);
	void setFrame(const class Leap::Frame &frame);
	const Leap::Frame &getFrame() const;

	virtual void CleanupRootReferences();

private:
	class PrivateLeapFrame* _private;
};