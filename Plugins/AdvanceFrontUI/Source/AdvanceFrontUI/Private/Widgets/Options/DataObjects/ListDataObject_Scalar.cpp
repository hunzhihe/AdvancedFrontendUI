// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
    FCommonNumberFormattingOptions Optiions;
    Optiions.MaximumFractionalDigits = NumFracDigit;
    return Optiions;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
	// 从 getter 读取原始值，从输出范围映射到显示范围
    if (DataDynamicGetter)
    {
        return FMath::GetMappedRangeValueClamped(
            OutputValueRange,
            DisplayValueRange,
            StringToFloat(DataDynamicGetter->GetValueAsString())
        );
    }


    return 0.0f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
    if (DataDynamicSetter)
    {
    	// 从显示范围映射回输出范围
        const float ClampedValue = FMath::GetMappedRangeValueClamped(
            DisplayValueRange,
            OutputValueRange,
            InNewValue

        );

		// 写入游戏设置并通知修改
        DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));

        NotifyListDataModified(this);
    }
}

bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	// 比较当前值与默认值，差距超过容差则允许重置
    if (HasDefaultValue() && DataDynamicGetter)
    {
        const float DefaultValue = StringToFloat(GetDefualtValueAsString());
        const float CurrentValum = StringToFloat(DataDynamicGetter->GetValueAsString());


        return !FMath::IsNearlyEqual(DefaultValue, CurrentValum, 0.01F);
    }
    return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{

    if (CanResetBackToDefaultValue())
    {
        if (DataDynamicSetter) 
        {
            DataDynamicSetter->SetValueFromString(GetDefualtValueAsString());

            NotifyListDataModified(this,EOptionsLsitDataModifyReason::ResetToDefault);

            return true;

        }
    }
    return false;
}

void UListDataObject_Scalar::OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason)
{
	// 依赖数据变更时，先通知自身数据已修改再调用父类广播
    NotifyListDataModified(this, EOptionsLsitDataModifyReason::DependencyModified);
    Super::OnEditDependencyDataModifiied(InModifiedDependencyData, ModifyReason);
}

float UListDataObject_Scalar::StringToFloat(const FString& InString) const
{
    float OutConvertedValue = 0.F;
    LexFromString(OutConvertedValue, InString);


    return OutConvertedValue;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
    FCommonNumberFormattingOptions Optiions;
    Optiions.MaximumFractionalDigits = 0;
    return Optiions;
}