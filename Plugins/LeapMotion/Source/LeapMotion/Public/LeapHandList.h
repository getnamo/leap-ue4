#pragma once

#include "LeapMotionPublicPCH.h"
#include "LeapHandList.generated.h"

UCLASS(BlueprintType)
class LEAPMOTION_API ULeapHandList : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	~ULeapHandList();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Hand List")
	bool IsEmpty;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Leap Hand List")
	int32 Count;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getFrontmost", CompactNodeTitle = "", Keywords = "get frontmost"), Category = "Leap Hand List")
	class ULeapHand* Frontmost();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getLeftmost", CompactNodeTitle = "", Keywords = "get leftmost"), Category = "Leap Hand List")
	class ULeapHand* Leftmost();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getRightmost", CompactNodeTitle = "", Keywords = "get rightmost"), Category = "Leap Hand List")
	class ULeapHand* Rightmost();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getIndex", CompactNodeTitle = "", Keywords = "get index"), Category = "Leap Hand List")
	class ULeapHand* getIndex(int32 index);

	void setHandList(const class Leap::HandList &handlist);

	virtual void CleanupRootReferences();

private:
	class PrivateHandList* _private;
};