// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"


class UFrontendUIGameUserSettings;
/**
 * 选项数据交互辅助器
 * 通过 FCachedPropertyPath 动态绑定 UFrontendUIGameUserSettings 的 getter/setter 函数，
 * 实现对任意游戏设置的字符串形式读写，避免硬编码设置名称。
 * 用于 UMyListDataObject_Value 及其子类与游戏用户设置之间的数据桥接。
 */
class ADVANCEFRONTUI_API FOptionsDataInteractionHelper
{
public:
	/**
	 * 构造函数
	 * @param InSettterOrGetterFuncPath 游戏用户设置中的 getter/setter 函数名（用于属性路径查找）
	 */
	FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath);

	/** 将游戏用户设置的当前值以字符串形式返回 */
	FString GetValueAsString() const;
	/** 将字符串值写入游戏用户设置 */
	void SetValueFromString(const FString& InValue) const;

private:
	/** 缓存的动态属性路径，指向目标 getter/setter 函数 */
	FCachedPropertyPath CachedDynamicFunctionPath;
	/** 缓存的游戏用户设置弱引用 */
	TWeakObjectPtr<UFrontendUIGameUserSettings> CachedWeakGameUserSettings;

};
