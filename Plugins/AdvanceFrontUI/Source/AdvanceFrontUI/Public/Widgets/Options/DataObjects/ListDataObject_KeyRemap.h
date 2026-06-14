// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "ListDataObject_KeyRemap.generated.h"




class UEnhancedInputUserSettings;
class UEnhancedPlayerMappableKeyProfile;
/**
 * 按键重映射类型的列表数据对象
 * 封装 Enhanced Input 系统中单个按键映射的读写操作。
 * 支持绑定新按键、获取按键图标、重置为默认按键。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_KeyRemap : public UListDataObject_Base
{
	GENERATED_BODY()
public:

	/** 初始化按键重映射数据 */
	void InitKeyRemapData(UEnhancedInputUserSettings* InOwningInputUserSettings,
		UEnhancedPlayerMappableKeyProfile* InKeyProfile, ECommonInputType InDesiredInputKeyType,
		const FPlayerKeyMapping& InOwningPlayerKeyMapping);

	/** 获取当前绑定按键的图标 Brush */
	FSlateBrush GetIconFromCurrentKey() const;

	/** 绑定新的输入按键 */
	void BindNewInputKey(const FKey& InNewKey);

	//Begin UListDataObject_Base Interface
	virtual bool HasDefaultValue() const override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;
	//End UListDataObject_Base Interface

private:

	/** 获取所属的 FPlayerKeyMapping 指针（通过 KeyProfile 查找） */
	FPlayerKeyMapping* GetOwningKeyMapping() const;

	/** 缓存的 Enhanced Input 用户设置 */
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	/** 缓存的按键映射配置文件 */
	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	/** 缓存的目标输入类型（键盘鼠标/手柄） */
	ECommonInputType CachedDesiredInputKeyType;

	/** 缓存的映射名称 */
	FName CachedOwningMappingName;

	/** 缓存的映射插槽 */
	EPlayerMappableKeySlot CachedOwningMappableKeySlot;

public:
	FORCEINLINE ECommonInputType GetDesiredInputKeyType() const
	{
		return CachedDesiredInputKeyType;
	}
};
