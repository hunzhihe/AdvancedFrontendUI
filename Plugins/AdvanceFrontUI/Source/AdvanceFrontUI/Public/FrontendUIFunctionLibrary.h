// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Internationalization/StringTableRegistry.h"
#include "Internationalization/Text.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "FrontendUIFunctionLibrary.generated.h"


class UWidget_ActivatableBase;
class UTexture2D;

/**
 * 前端 UI 蓝图函数库
 * 提供静态辅助函数，通过 GameplayTag 从 UFrontendDeveloperSettings 中
 * 查找对应的控件类或图片资产。
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:

	/**
	 * 根据 GameplayTag 获取前端控件类（软引用）
	 * 从 UFrontendDeveloperSettings::FrontendWidgetMap 中查找
	 * @param WidgetTag 控件标签（受 "Frontend.Widget" 类别限制）
	 * @return 对应的控件类软引用，未找到则为空
	 */
	UFUNCTION(BlueprintPure, Category = "Frontend UI Functions library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendWidgetFromTag(
	 UPARAM(meta = (Categories = "Frontend.Widget"))FGameplayTag WidgetTag);

	/**
	 * 根据 GameplayTag 获取选项画面的图片资产（软引用）
	 * 从 UFrontendDeveloperSettings::OptionsScreenSoftImageMap 中查找
	 * @param ImageTag 图片标签（受 "Frontend.Image" 类别限制）
	 * @return 对应的纹理软引用
	 */
	UFUNCTION(BlueprintPure, Category = "Frontend UI Functions library")
	static TSoftObjectPtr<UTexture2D>GetOptionsSoftImage(
		UPARAM(meta = (Categories = "Frontend.Image"))FGameplayTag ImageTag);

	UFUNCTION(BlueprintPure, Category = "Frontend UI Functions library")
	static FText GetCurrentLanguageTextFromTable(const ELaughageChanged CurrentLanguage, const FString& Key);

};
