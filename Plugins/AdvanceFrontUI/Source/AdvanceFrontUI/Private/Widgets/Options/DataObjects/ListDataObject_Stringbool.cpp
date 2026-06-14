// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Stringbool.h"


void UListDataObject_Stringbool::OverrideTrueDisplayText(const FText& InNameTrueDisplayText)
{
	if (!AvaiableOptionsStringArray.Contains(TrusString))
	{
		AddDynamicOption(TrusString, InNameTrueDisplayText);
	}
}

void UListDataObject_Stringbool::OverrideFalseDisplayText(const FText& InNameFalseDisplayText)
{
	if (!AvaiableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, InNameFalseDisplayText);
	}
}

void UListDataObject_Stringbool::SetTrueAsDefaultValue()
{
	SetDefaultValueFromString(TrusString);
}

void UListDataObject_Stringbool::SetFalseAsDefaultValue()
{
	SetDefaultValueFromString(FalseString);
}

void UListDataObject_Stringbool::OnDataObjectInitialized()
{
	TryInitBoolValue();

	Super::OnDataObjectInitialized();
}


void UListDataObject_Stringbool::TryInitBoolValue()
{
	// 如果尚未通过 Override 函数自定义，则使用默认的 ON/OFF 显示文本
	if (!AvaiableOptionsStringArray.Contains(TrusString))
	{
		AddDynamicOption(TrusString, FText::FromString(TEXT("ON")));
	}

	if (!AvaiableOptionsStringArray.Contains(FalseString))
	{
		AddDynamicOption(FalseString, FText::FromString(TEXT("OFF")));
	}

}


