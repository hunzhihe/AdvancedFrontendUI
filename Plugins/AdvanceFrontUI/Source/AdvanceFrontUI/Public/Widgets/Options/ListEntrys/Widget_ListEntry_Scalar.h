// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;
class UListDataObject_Scalar;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEFRONTUI_API UWidget_ListEntry_Scalar : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//End UUserWidget Interface

	//Begin UWidget_ListEntry_Base interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject) override;
	virtual void OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason  ModifyReason) override;
	//End UWidget_ListEntry_Base interface


private:
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnSliderMouseCaptureBegin();

	//Bound Widget
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_SettingSlider;

	//Bound Widget
    
	UPROPERTY(Transient)
	UListDataObject_Scalar* CachedOwningScalarDataObject;
};
