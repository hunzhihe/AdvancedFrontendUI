// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "FrontendCommonRotator.generated.h"

/**
 * 前端通用 Rotator 控件
 * 继承自 UCommonRotator，增加通过文本匹配选择选项的功能。
 * 用于在预设文本标签列表中定位并选中指定项。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEFRONTUI_API UFrontendCommonRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	/**
	 * 根据显示文本选中 Rotator 中的选项
	 * 遍历 TextLabels 数组查找匹配项，找到则选中；未找到则直接在 MyText 上显示该文本
	 */
	void SetSelectedOptionByText(const FText& OptionText);
};
