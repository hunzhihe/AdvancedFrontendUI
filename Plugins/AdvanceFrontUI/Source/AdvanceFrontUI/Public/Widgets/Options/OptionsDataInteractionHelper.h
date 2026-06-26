// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"


class UFrontendUIGameUserSettings;
/**
 * 选项数据交互辅助器
 * 通过 FCachedPropertyPath 动态绑定目标对象的 getter/setter 函数，
 * 实现对任意设置的字符串形式读写，避免硬编码设置名称。
 *
 * - 无参构造函数：默认绑定 UFrontendUIGameUserSettings 单例（兼容旧代码）
 * - 有参构造函数：绑定任意 UObject 实例（如 UPlayerSaveData）
 *
 * 用于 UMyListDataObject_Value 及其子类与目标数据对象之间的数据桥接。
 */
class ADVANCEFRONTUI_API FOptionsDataInteractionHelper
{
public:
    /**
     * 构造函数（兼容旧代码）
     * 默认绑定到 UFrontendUIGameUserSettings 单例
     * @param InSettterOrGetterFuncPath getter/setter 函数名（用于属性路径查找）
     */
    FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath);

    /**
     * 构造函数（新）
     * 绑定到指定的目标对象实例
     * @param InSettterOrGetterFuncPath getter/setter 函数名
     * @param InTargetObject 目标对象实例（如 UPlayerSaveData）
     */
    FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath, UObject* InTargetObject);

    /** 将目标对象当前值以字符串形式返回 */
    FString GetValueAsString() const;
    /** 将字符串值写入目标对象 */
    void SetValueFromString(const FString& InValue) const;

    /** 获取当前绑定的目标对象 */
    UObject* GetTargetObject() const { return CachedWeakTargetObject.Get(); }

private:
    /** 缓存的动态属性路径，指向目标 getter/setter 函数 */
    FCachedPropertyPath CachedDynamicFunctionPath;
    /** 缓存的目标对象弱引用 */
    TWeakObjectPtr<UObject> CachedWeakTargetObject;
};
