// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/** 确认画面类型枚举 */
UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
     OK,           // 仅确定
	 YesNo,        // 是/否
	 OkCancel,     // 确定/取消
	 Unknown UMETA(Hidden)  // 未知（隐藏）
};

/** 确认画面按钮点击类型枚举 */
UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
     Confirmed,    // 确认/是
	 Cancelled,    // 取消/否
	 Closed,       // 关闭
	 Unknown UMETA(Hidden)  // 未知（隐藏）
};

/** 选项列表数据修改原因枚举 */
UENUM(BlueprintType)
enum class EOptionsLsitDataModifyReason : uint8
{
	DirectlyModified,   // 直接修改（用户主动操作）
	DependencyModified, // 依赖修改（由关联数据变更触发）
	ResetToDefault,     // 重置为默认值
	Unknown UMETA(Hidden) // 未知（隐藏）
};

UENUM(BlueprintType)
enum class ELaughageChanged : uint8
{
	ZH_Ch,   
	English,   
	Unknown UMETA(Hidden) // 未知（隐藏）
};