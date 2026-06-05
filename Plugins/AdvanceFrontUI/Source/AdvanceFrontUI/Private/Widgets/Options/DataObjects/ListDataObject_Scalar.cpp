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