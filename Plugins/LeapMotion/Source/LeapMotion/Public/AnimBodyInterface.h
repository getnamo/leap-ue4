
#pragma once

#include "AnimBody.h"
#include "AnimBodyInterface.generated.h"

UINTERFACE()
class LEAPMOTION_API UAnimBodyInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IAnimBodyInterface
{
	GENERATED_IINTERFACE_BODY()

public:

	/*UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "HandCountChanged", CompactNodeTitle = "", Keywords = "hand count"), Category = "Leap Interface Event")
		void HandCountChanged(int32 count);
	virtual void OnHandCountChanged(int32 count) = 0;
	*/
	virtual UAnimBody* GetAnimBody() = 0;

	//Help identifying
	virtual FString ToString() { return "IAnimBodyInterface::ToString()"; };
};

