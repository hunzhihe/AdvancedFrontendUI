// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_ConformScreen.generated.h"


class UCommonTextBlock;
class UDynamicEntryBox;


USTRUCT(BlueprintType)
struct FConformScreenButtonInfo
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;


};
/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()
	public:

	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage);
    static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
    static UConfirmScreenInfoObject* CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	UPROPERTY(Transient)
	FText ScreenTitle;

	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	TArray<FConformScreenButtonInfo> AvailableDcreenButtons;

};





UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_ConformScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
	public:
	void InitConformScreen(
		UConfirmScreenInfoObject* InConfirmScreenInfoObject,TFunction<void(EConfirmScreenButtonType)> InButtonClickedCallback);


	private:
	UPROPERTY(META = (BindWidget))
	UCommonTextBlock* CommonTextBlock_ConformScreenTitle;

	UPROPERTY(META = (BindWidget))
	UCommonTextBlock* CommonTextBlock_ConformScreenMessage;

	UPROPERTY(META = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_ConformScreenButton;
};
