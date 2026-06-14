// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 分辨率类型的字符串列表数据对象
 * 自动枚举系统支持的所有全屏分辨率，按从小到大排序。
 * 支持依赖 WindowMode 的编辑条件（非全屏模式下根据窗口模式自动限制可选分辨率）。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	/** 初始化分辨率选项列表（从系统获取所有支持的全屏分辨率） */
	void InitResolutionValues();


protected:
	//Begin UListDataObject_String interface
	virtual void OnDataObjectInitialized() override;
	//Begin UListDataObject_String interface

private:
	/** 将 FIntPoint 分辨率转换为字符串值格式 "(X=...,Y=...)" */
	FString ResToValueString(const FIntPoint& InResolution) const;
	/** 将 FIntPoint 分辨率转换为显示文本格式 "(宽 x 高)" */
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	/** 最大允许分辨率（字符串格式），用于非全屏窗口模式下的强制值 */
	FString MaximumAllowedResolution;

public:
	FORCEINLINE FString GetMaximumAllowedResolution() const {
		return MaximumAllowedResolution;
	}
};
