// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_Scalar.generated.h"

class UCommonNumericTextBlock;
class UAnalogSlider;
class UListDataObject_Scalar;
/**
 * 标量（滑块）列表条目控件
 * 显示一个 AnalogSlider 和对应的数值文本，绑定到 UListDataObject_Scalar。
 * 支持拖拽滑块修改值、实时更新显示数值。
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
	/** 滑块值变化回调 */
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	/** 滑块开始拖拽时的回调（自动选中此条目） */
	UFUNCTION()
	void OnSliderMouseCaptureBegin();

	//Bound Widget — 由蓝图绑定的子控件
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UCommonNumericTextBlock* CommonNumeric_SettingValue;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UAnalogSlider* AnalogSlider_SettingSlider;

	//Bound Widget

	/** 缓存的所属标量数据对象 */
	UPROPERTY(Transient)
	UListDataObject_Scalar* CachedOwningScalarDataObject;
};
