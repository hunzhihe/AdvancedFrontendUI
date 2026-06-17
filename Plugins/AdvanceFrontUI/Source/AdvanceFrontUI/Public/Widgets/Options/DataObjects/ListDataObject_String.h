// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/MyListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 字符串类型的列表数据对象
 * 通过 Rotator 控件在预设选项列表中切换选择。
 * 支持动态选项添加、前进/后退切换、通过显示文本反查选中项。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_String : public UMyListDataObject_Value
{
	GENERATED_BODY()

public:

	/** 添加一个动态选项（字符串值与显示文本的映射） */
	void AddDynamicOption(const FString& OptionString, const FText& OptionDisplayText);

	/** 切换到下一个选项（循环） */
	void AdvanceToNextOption();
	/** 切换到上一个选项（循环） */
	void BackToPerviousOption();

	/** 当 Rotator 控件触发值变化时调用 */
	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);

protected:
	//Begin UListDataObject_Base interface
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue()  override;

	/** 判断是否支持被强制设置为某个字符串值 */
	virtual bool CanSetToForcedStringvalue(const FString& InForcedValue) const override;
	/** 当被强制设置值时调用 */
	virtual void  OnSetToForcedStringvalue(const FString& InForcedValue) override;
	//End UListDataObject_Base interface

	/** 根据字符串值查找并设置对应的显示文本 */
	bool TrySetDisplayTextByStringValue(const FString& InStringValue);

	/** 当前选中值的字符串形式 */
	FString CurrentStringValue;
	/** 当前选中值对应的显示文本 */
	FText CurrentDisplayText;
	/** 所有可用选项的字符串值数组 */
	TArray<FString> AvaiableOptionsStringArray;
	/** 所有可用选项的显示文本数组 */
	TArray<FText> AvaiableOptionsDisplayTextArray;

public:

	FORCEINLINE const TArray<FText>& GetAvaiableOptionsDisplayTextArray() const { return AvaiableOptionsDisplayTextArray; }
	FORCEINLINE const FString& GetCurrentStringValue() const { return CurrentStringValue; }
	FORCEINLINE const FText& GetCurrentDisplayText() const { return CurrentDisplayText; }
	FORCEINLINE const TArray<FString>& GetAvaiableOptionsStringArray() const { return AvaiableOptionsStringArray; }

};

/**
 * 枚举类型的字符串列表数据对象
 * 提供模板方法将枚举值自动转换为字符串选项。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()

public:

	

	/** 添加一个枚举选项 */
	template<typename EnumType>
	void AddEnumOptions(EnumType InEnumOption, const FText& InDispalyText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const int64  EnumValue = static_cast<int64>(InEnumOption);
		const FString ConveretedEnumString =  StaticEnumOption->GetNameStringByValue(EnumValue);

		AddDynamicOption(ConveretedEnumString, InDispalyText);

	}

	/** 获取当前选中值作为枚举值 */
	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	/** 设置默认值（枚举形式） */
	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConveretedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConveretedEnumString);
	}
};

/**
 * 整数类型的字符串列表数据对象
 * 用于图形质量等级等整数选项，当值不在预设列表中时显示"Custom"。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_StringInteger : public UListDataObject_String
{

	GENERATED_BODY()

public:
	/** 添加一个整数选项 */
	void AddIntegerOption(int32 InIntegerValue, const FText& InDispalyText);

protected:

	//Begin UListDataObject_String interface
	virtual void OnDataObjectInitialized() override;

	/** 当依赖数据被修改时，同步刷新自身的当前值 */
	virtual void OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason) override;
	//End UListDataObject_String interface
private:



};
