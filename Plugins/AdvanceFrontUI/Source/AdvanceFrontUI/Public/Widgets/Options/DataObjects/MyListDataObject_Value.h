// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "MyListDataObject_Value.generated.h"


class FOptionsDataInteractionHelper;
/**
 * 带值的列表数据对象抽象基类
 * 在 UListDataObject_Base 基础上增加了 getter/setter 支持，
 * 通过 FOptionsDataInteractionHelper 实现与游戏设置的动态属性路径绑定。
 * 所有需要读写实际设置值的选项数据对象都继承自此类。
 */
UCLASS(Abstract)
class ADVANCEFRONTUI_API UMyListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
public:
	/** 设置用于读取值的动态 getter */
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	/** 设置用于写入值的动态 setter */
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	/** 设置默认值（字符串形式，用于重置功能） */
	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }

	/** 是否有默认值 */
	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }


protected:
	/** 获取默认值字符串 */
	FString GetDefualtValueAsString() const { return DefaultStringValue.GetValue(); }

	/** 数据读取器（动态属性路径绑定） */
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	/** 数据写入器（动态属性路径绑定） */
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	/** 默认值字符串（可选，未设置表示无默认值） */
	TOptional<FString> DefaultStringValue;

};
