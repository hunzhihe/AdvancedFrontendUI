// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath)
	:CachedDynamicFunctionPath(InSettterOrGetterFuncPath)
{
	// 缓存游戏用户设置单例的弱引用
	CachedWeakGameUserSettings = UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings();

}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
	FString OutStringValue;
	// 通过动态属性路径从游戏用户设置读取值
	PropertyPathHelpers::GetPropertyValueAsString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath, OutStringValue);
	return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InValue) const
{
	// 通过动态属性路径向游戏用户设置写入值
	PropertyPathHelpers::SetPropertyValueFromString(
		CachedWeakGameUserSettings.Get(),
		CachedDynamicFunctionPath,
		InValue
		);
}
