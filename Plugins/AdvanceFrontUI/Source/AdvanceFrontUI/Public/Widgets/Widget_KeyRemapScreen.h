// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "CommonInputTypeEnum.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

protected:

	//Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//End UCommonActivatableWidget Interface


private:

	void OnValidKeyPressedDelected(const FKey& PressKey);
	void OnKeySelectCanceled(const FString& CanceledReason);

	//Bound Widgets
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage;
	//Bound Widgets

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;
	
	ECommonInputType CachedDesiredInputType;
};
