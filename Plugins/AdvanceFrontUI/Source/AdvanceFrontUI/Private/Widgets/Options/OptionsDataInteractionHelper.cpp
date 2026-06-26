// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath)
    : CachedDynamicFunctionPath(InSettterOrGetterFuncPath)
{
    // 默认绑定到 UFrontendUIGameUserSettings 单例（兼容旧代码）
    CachedWeakTargetObject = UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings();
}

FOptionsDataInteractionHelper::FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath, UObject* InTargetObject)
    : CachedDynamicFunctionPath(InSettterOrGetterFuncPath)
{
    // 绑定到指定的目标对象实例
    CachedWeakTargetObject = InTargetObject;
}

FString FOptionsDataInteractionHelper::GetValueAsString() const
{
    FString OutStringValue;
    // 通过动态属性路径从目标对象读取值
    PropertyPathHelpers::GetPropertyValueAsString(
        CachedWeakTargetObject.Get(),
        CachedDynamicFunctionPath, OutStringValue);
    return OutStringValue;
}

void FOptionsDataInteractionHelper::SetValueFromString(const FString& InValue) const
{
    // 通过动态属性路径向目标对象写入值
    PropertyPathHelpers::SetPropertyValueFromString(
        CachedWeakTargetObject.Get(),
        CachedDynamicFunctionPath,
        InValue);
}
