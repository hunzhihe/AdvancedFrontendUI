// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_Stringbool.generated.h"

/**
 * 布尔类型的字符串列表数据对象
 * 内部使用 "true"/"false" 字符串表示布尔值，
 * 支持自定义真/假值的显示文本（如 "Enable"/"Disabled"）。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_Stringbool : public UListDataObject_String
{
	GENERATED_BODY()


public:
	/** 覆盖 true 值的显示文本 */
	void OverrideTrueDisplayText(const FText& InNameTrueDisplayText);
	/** 覆盖 false 值的显示文本 */
	void OverrideFalseDisplayText(const FText& InNameFalseDisplayText);

	/** 设置 true 选项的本地化键（StringTable 查表键，覆盖默认的 "true"） */
	void SetTrueLocalizationKey(const FString& InKey);
	/** 设置 false 选项的本地化键（StringTable 查表键，覆盖默认的 "false"） */
	void SetFalseLocalizationKey(const FString& InKey);

	/** 设置 true 为默认值 */
	void SetTrueAsDefaultValue();
	/** 设置 false 为默认值 */
	void SetFalseAsDefaultValue();

protected:

	//UListDataObject_String Interface
	virtual void OnDataObjectInitialized() override;
	virtual void RefreshLocalizedText() override;
	//UListDataObject_String Interface

private:
	/** 初始化布尔值选项（ON/OFF），可通过 Override 函数自定义 */
	void TryInitBoolValue();

	const FString TrusString = TEXT("true");
	const FString FalseString = TEXT("false");

	/** true 选项的本地化键（延迟应用，RefreshLocalizedText 时设置到数组） */
	FString TrueLocalizationKey;
	/** false 选项的本地化键 */
	FString FalseLocalizationKey;

};
