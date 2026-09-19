// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionEnums.generated.h"

// ============================================================================
// 交互系统枚举（由蓝图 UserDefinedEnum 转换而来）
// 原资产路径：/Game/InteractionManager/Blueprints/Data/Enums/
//   - Enum_InteractionType
//   - Enum_InteractionState
//   - Enum_InteractionNetworkHandleMethod
//   - Enum_InteractionFinishMethod
//   - Enum_InteractionResult
// 说明：
//   - 枚举值与蓝图中的顺序一致（0, 1, 2, ...），用于序列化/网络同步时保持一致。
//   - 通过 UMETA(DisplayName = "...") 保留蓝图里的原始显示名。
// ============================================================================

// ----------------------------------------------------------------------------
// 交互类型：Tap=点按 / Hold=长按 / Repeat=重复
// ----------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	Tap     UMETA(DisplayName = "Tap"),
	Hold    UMETA(DisplayName = "Hold"),
	Repeat  UMETA(DisplayName = "Repeat"),
};

// ----------------------------------------------------------------------------
// 交互状态：等待 / 交互中 / 完成
// 注意：第 4 个条目在蓝图里显示名称为空（疑似占位或 MAX），此处命名为 None，请按需调整。
// ----------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EInteractionState : uint8
{
	Waiting      UMETA(DisplayName = "Waiting"),
	Interacting  UMETA(DisplayName = "Interacting"),
	Done         UMETA(DisplayName = "Done"),

	/** 显示名称为空，用途待确认（请按需重命名） */
	None         UMETA(DisplayName = ""),
};

// ----------------------------------------------------------------------------
// 网络处理方式：交互期间保持启用 / 交互期间禁用
// ----------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EInteractionNetworkHandleMethod : uint8
{
	KeepEnabled              UMETA(DisplayName = "Keep Enabled"),
	DisableWhileInteracting  UMETA(DisplayName = "Disable While Interacting"),
};

// ----------------------------------------------------------------------------
// 交互结果：完成 / 取消
// ----------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EInteractionResult : uint8
{
	Completed  UMETA(DisplayName = "Completed"),
	Canceled   UMETA(DisplayName = "Canceled"),
};

// ----------------------------------------------------------------------------
// 交互结束方式：交互完成后对目标执行的动作（销毁/停用/延时重新激活），含两两组合
// ----------------------------------------------------------------------------
UENUM(BlueprintType)
enum class EInteractionFinishMethod : uint8
{
	DestroyOnCompleted                              UMETA(DisplayName = "Destroy On Completed"),
	DestroyOnCanceled                               UMETA(DisplayName = "Destroy On Canceled"),
	DestroyOnCompletedOrCanceled                    UMETA(DisplayName = "Destroy On Completed Or Canceled"),
	ReactivateAfterDurationOnCompleted              UMETA(DisplayName = "Reactivate After Duration On Completed"),
	ReactivateAfterDurationOnCanceled               UMETA(DisplayName = "Reactivate After Duration On Canceled"),
	ReactivateAfterDurationOnCompletedOrCanceled    UMETA(DisplayName = "Reactivate After Duration On Completed Or Canceled"),
	DeactivateOnCompleted                           UMETA(DisplayName = "Deactivate On Completed"),
	DeactivateOnCanceled                            UMETA(DisplayName = "Deactivate On Canceled"),
	DeactivateOnCompletedOrCanceled                 UMETA(DisplayName = "Deactivate On Completed Or Canceled"),

	ReactivateAfterDurationOnCanceled_DestroyOnCompleted           UMETA(DisplayName = "Reactivate After Duration On Canceled, Destroy On Completed"),
	ReactivateAfterDurationOnCanceled_DeactivateOnCompleted        UMETA(DisplayName = "Reactivate After Duration On Canceled, Deactivate On Completed"),
	DeactivateOnCanceled_ReactivateAfterDurationOnCompleted        UMETA(DisplayName = "Deactivate On Canceled, Reactivate After Duration On Completed"),
	DeactivateOnCanceled_DestroyOnCompleted                        UMETA(DisplayName = "Deactivate On Canceled, Destroy On Completed"),
	ReactivateAfterDurationOnCompleted_DestroyOnCanceled           UMETA(DisplayName = "Reactivate After Duration On Completed, Destroy On Canceled"),
	ReactivateAfterDurationOnCompleted_DeactivateOnCanceled        UMETA(DisplayName = "Reactivate After Duration On Completed, Deactivate On Canceled"),
	DeactivateOnCompleted_ReactivateAfterDurationOnCanceled        UMETA(DisplayName = "Deactivate On Completed, Reactivate After Duration On Canceled"),
	DeactivateOnCompleted_DestroyOnCanceled                        UMETA(DisplayName = "Deactivate On Completed, Destroy On Canceled"),
};
