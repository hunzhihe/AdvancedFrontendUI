// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath)
	:CachedDynamicFunctionPath(InSettterOrGetterFuncPath)
{
	CachedWeakGameUserSettings = UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings();

}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(), 
		CachedDynamicFunctionPath, OutStringValue);
	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InValue) const
{
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InValue
		);
}
