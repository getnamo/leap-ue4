// Created by Chance_07 2014
#pragma once

#include "AnimGraphNode_Base.h"
#include "AnimGraphDefinitions.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimNode_TranslateWith.h"
#include "AnimGraphNode_TranslateWith.generated.h"



/**
*
*/
UCLASS(MinimalAPI)
class UAnimGraphNode_TranslateWith : public UAnimGraphNode_Base
{
	GENERATED_UCLASS_BODY()

		UPROPERTY(EditAnywhere, Category = SkeletalControl)
		FAnimNode_TranslateWith Node;

public:
	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	virtual FString GetNodeCategory() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void CreateOutputPins() override;
	// End of UEdGraphNode interface

protected:
	virtual FText GetControllerDescription() const;

};