// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/MyListDataObject_Value.h"
#include "CommonNumericTextBlock.h"
#include "ListDataObject_Scalar.generated.h"

/**
 * 标量（浮点）类型的列表数据对象
 * 用于音量、亮度等连续值设置，显示为滑块控件。
 * 支持显示值范围与实际输出值范围的映射（如滑块 0-1 映射到音量 0-10）。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_Scalar : public UMyListDataObject_Value
{
	GENERATED_BODY()

public:

	LIST_DATA_ACCESSOR(TRange<float>, DisplayValueRange)
	LIST_DATA_ACCESSOR(TRange<float>, OutputValueRange)
	LIST_DATA_ACCESSOR(float, SliderStepSize)
	LIST_DATA_ACCESSOR(ECommonNumericType, DisplayNumericType)
	LIST_DATA_ACCESSOR(FCommonNumberFormattingOptions, NumberFormattingOptions)

	/** 创建无小数位的格式化选项 */
	static FCommonNumberFormattingOptions NoDecimal();
	/** 创建指定小数位数的格式化选项 */
	static FCommonNumberFormattingOptions WithDecimal(int32 NumFracDigit);

	/** 获取当前值（从输出范围映射到显示范围） */
	float GetCurrentValue() const;
	/** 通过滑块设置当前值（从显示范围映射到输出范围） */
	void SetCurrentValueFromSlider(float InNewValue);

protected:




private:

	//Begin UListDataObject_Base interface
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue()  override;
	virtual void OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason) override;
	//End UListDataObject_Base interface

	/** 将字符串转换为浮点数 */
	float StringToFloat(const FString& InString) const;

	TRange<float> DisplayValueRange = TRange<float>(0.f, 1.f);
	TRange<float> OutputValueRange  =  TRange<float>(0.f, 1.f);
	float SliderStepSize = 0.1f;
	ECommonNumericType DisplayNumericType = ECommonNumericType::Number;
	FCommonNumberFormattingOptions NumberFormattingOptions;
};
