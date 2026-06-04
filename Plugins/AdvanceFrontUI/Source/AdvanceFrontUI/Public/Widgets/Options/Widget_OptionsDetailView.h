// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailView.generated.h"


class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;
class UListDataObject_Base;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEFRONTUI_API UWidget_OptionsDetailView : public UUserWidget
{
	GENERATED_BODY()


public:	
	void UpdateDetailView(const UListDataObject_Base* InDataObject,const FString& InEntryWidgetClassName = FString());
	void ClearDetailView();

protected:
	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//End UUserWidget Interface

private:
	//Bound Widgets
	UPROPERTY(meta =(BindWidget))
	UCommonTextBlock* TextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_Description;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DisableReason;


	//Bound Widgtes
	
};
