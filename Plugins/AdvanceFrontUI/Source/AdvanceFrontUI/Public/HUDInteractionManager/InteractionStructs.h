// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionStructs.generated.h"

// ============================================================================
// 交互系统结构体（由蓝图 UserDefinedStruct 转换而来）
// 原资产路径：/Game/InteractionManager/Blueprints/Data/Structs/
//   - Struct_IconSelector        图标选择器
//   - Struct_PendingTargetData   待处理目标数据
// ============================================================================

// 前向声明
class UTexture2D;
class UMaterialInstance;
class UManager_InteractionTarget;

// ----------------------------------------------------------------------------
// 图标选择器：用于 Manager_InteractionTarget 的 Target_Icon / POI_Icon /
// Interaction_Icon_Background / Interaction_Icon_Foreground 四个图标配置。
// 图标可由纹理（Texture2D）或材质（Material）二选一，用 UseMaterialAsTexture 切换。
// ----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FStructIconSelector
{
	GENERATED_BODY()

	/** 是否使用材质作为图标（true=用 Material，false=用 Texture2D） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Selector", meta = (DisplayName = "Use Material As Texture"))
	bool UseMaterialAsTexture = false;

	/** 图标纹理（2D，硬引用） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Selector", meta = (DisplayName = "Texture 2D"))
	UTexture2D* Texture2D = nullptr;

	/** 图标材质实例（软引用；若蓝图里实为硬引用，请改为 UMaterialInstance*） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon Selector", meta = (DisplayName = "Material"))
	TSoftObjectPtr<UMaterialInstance> Material;
};

// ----------------------------------------------------------------------------
// 待处理目标数据：记录一个待处理（延迟交互 / 重新激活）的目标组件及其被交互时的游戏时间。
// 说明：该结构体并非 Manager_Interactor / Manager_InteractionTarget 的直接成员，
//       多为蓝图内部（如宏库 MacroLibrary_Interaction）使用，此处一并转换备用。
// 字段顺序依据蓝图 DisplayName 序列化顺序推断，请核对与原蓝图一致。
// ----------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FStructPendingTargetData
{
	GENERATED_BODY()

	/** 被交互时的游戏时间（用于判定是否到达重新激活时长） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pending Target Data", meta = (DisplayName = "Interacted Game Time"))
	double InteractedGameTime = 0.0;

	/** 目标组件（交互目标） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pending Target Data", meta = (DisplayName = "Target Component"))
	UManager_InteractionTarget* TargetComponent = nullptr;
};
