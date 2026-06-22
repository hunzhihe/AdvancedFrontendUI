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

void UListDataObject_Stringbool::SetTrueLocalizationKey(const FString& InKey)
{
	TrueLocalizationKey = InKey;
	// 如果 "true" 选项已在数组中（通过 OverrideTrueDisplayText 提前添加），立即应用键
	SetOptionLocalizationKey(TrusString, InKey);
}

void UListDataObject_Stringbool::SetFalseLocalizationKey(const FString& InKey)
{
	FalseLocalizationKey = InKey;
	SetOptionLocalizationKey(FalseString, InKey);
}

void UListDataObject_Stringbool::OnDataObjectInitialized()
{
	TryInitBoolValue();

	Super::OnDataObjectInitialized();
}

void UListDataObject_Stringbool::RefreshLocalizedText()
{
	// 确保本地化键已应用到选项数组（此时 TryInitBoolValue 一定已执行，"true"/"false" 在数组中）
	if (!TrueLocalizationKey.IsEmpty())
	{
		SetOptionLocalizationKey(TrusString, TrueLocalizationKey);
	}
	if (!FalseLocalizationKey.IsEmpty())
	{
		SetOptionLocalizationKey(FalseString, FalseLocalizationKey);
	}

	Super::RefreshLocalizedText();
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
