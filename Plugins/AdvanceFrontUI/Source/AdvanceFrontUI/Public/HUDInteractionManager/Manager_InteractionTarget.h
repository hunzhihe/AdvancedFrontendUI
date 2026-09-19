// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"			// FKey
#include "Layout/Margin.h"			// FMargin
#include "Engine/EngineTypes.h"		// FHitResult
#include "HUDInteractionManager/InteractionEnums.h"	// 交互系统枚举
#include "HUDInteractionManager/InteractionStructs.h"	// 交互系统结构体
#include "Manager_InteractionTarget.generated.h"

// ============================================================================
// 蓝图交互目标（InteractionTarget）的 C++ 声明骨架
// 原蓝图：/Game/InteractionManager/Blueprints/Components/Manager_InteractionTarget
// 说明：
//   - 本类描述一个「可交互目标」，含内外两层球形触发区、高亮组件、marker 图标、
//     交互文本、进度/冷却/重复等配置，并通过 5 个事件委托对外广播交互生命周期。
//   - 蓝图里引用到的枚举已转换为 UENUM，见 InteractionEnums.h：
//       Enum_InteractionState  -> EInteractionState（InteractionText 的 Map 键）
//       Enum_InteractionType   -> EInteractionType（成员 InteractionType）
//       Enum_InteractionNetworkHandleMethod -> EInteractionNetworkHandleMethod（成员 NetworkHandleMethod）
//       Enum_InteractionFinishMethod -> EInteractionFinishMethod（成员 FinishMethod）
//       Enum_InteractionResult -> EInteractionResult（OnInteractionEnd 委托的 Result 参数）
//   - 蓝图结构体 Struct_IconSelector / Struct_PendingTargetData 已转换为 USTRUCT，见 InteractionStructs.h。
//   - 蓝图里的 float/double 混用（UE5 蓝图 float 引脚默认即 double），此处统一为 float。
// ============================================================================

// 前向声明
class USphereComponent;
class USceneComponent;
class UPrimitiveComponent;
class AActor;
class APawn;
class AController;
class UManager_Interactor;  // 前置声明

// ----------------------------------------------------------------------------
// 事件委托签名（对应蓝图 Event Dispatcher）
// ----------------------------------------------------------------------------

/** 交互开始：携带发起交互的 Pawn */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionBeginSignature, APawn*, InteractorPawn);

/** 交互结束：携带交互结果（Enum_InteractionResult）与发起交互的 Pawn */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionEndSignature, EInteractionResult, Result, APawn*, InteractorPawn);

/** 交互进度更新：携带进度（0~1）、已重复次数与发起交互的 Pawn */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInteractionUpdatedSignature, float, Alpha, int32, Repeated, APawn*, InteractorPawn);

/** 交互重新激活：携带重新激活的目标 Pawn */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionReactivatedSignature, APawn*, ForPawn);

/** 交互停用：无参数 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionDeactivatedSignature);

/**
 * 交互目标组件（InteractionTarget）
 * 挂载在场景中的可交互物体上，负责维护触发区、高亮、图标、交互配置，
 * 并通过事件委托向外部（Interactor / 蓝图）广播交互状态变化。
 */
UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVANCEFRONTUI_API UManager_InteractionTarget : public UActorComponent
{
	GENERATED_BODY()

public:
	UManager_InteractionTarget();


	// ========================================================================
	// 事件委托（Event Dispatcher，可在蓝图中 Bind / Assign）
	// ========================================================================

	/** 交互开始时广播 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction Target|Events")
	FOnInteractionBeginSignature OnInteractionBegin;

	/** 交互结束时广播 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction Target|Events")
	FOnInteractionEndSignature OnInteractionEnd;

	/** 交互进度更新时广播 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction Target|Events")
	FOnInteractionUpdatedSignature OnInteractionUpdated;

	/** 交互重新激活时广播 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction Target|Events")
	FOnInteractionReactivatedSignature OnInteractionReactivated;

	/** 交互停用时广播 */
	UPROPERTY(BlueprintAssignable, Category = "Interaction Target|Events")
	FOnInteractionDeactivatedSignature OnInteractionDeactivated;

    

	// ========================================================================
	// 构造 / 初始化
	// ========================================================================

	/** 构造高亮组件列表（将 ComponentsToHighlight 中的组件加入 HighlightedComponents） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Construct_HighlightedComponents();

	/** 构造内/外两层球形触发区（InnerZone / OuterZone） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Construct_Overlap_Zones();

	/** 构造所有者要素（缓存 OwnerReference、MarkerTargetComponent 等） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Construct_Owner_Essentials();

	/** 更新控件信息（边距与屏幕半径百分比，供 marker 渲染使用） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Update_Widget_Info(FMargin InWidgetMargin, float InScreenRadiusPercent);

	// ========================================================================
	// 交互状态 / 配置查询
	// ========================================================================

	/** 交互是否启用（原蓝图函数名 IsInteractionEnabled?） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	bool IsInteractionEnabled();

	/** 目标被停用时的处理（清理状态、重置高亮等） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void On_Deactivated();

	/**
	 * 分配 / 取消分配交互器。
	 * @param Add       true 添加，false 移除
	 * @param Interactor 交互器控制器（AssignedInteractors 为 TArray<AController*>）
	 */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Assign_Interactor(bool Add, AController* Interactor);

	/** 目标被加入待处理（Pending）列表时的回调 */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void On_Added_To_Pending_Target();

	/** 是否启用重新激活（原蓝图函数名 IsReactivationEnabled?） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	bool IsReactivationEnabled();

	/** 松开按键时是否取消交互（原蓝图函数名 Cancel_On_Release?） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	bool Cancel_On_Release();

	/** 设置高亮开关 */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Set_Highlight(bool IsHighlighted);

	/** 启用 / 禁用交互 */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Enable_Interaction(bool Enabled);

	/** 按名称选择标记组件（MarkerTargetComponent） */
	UFUNCTION(BlueprintCallable, Category = "Interaction Target")
	void Select_MarkerComponent(FString ComponentName);

	// ========================================================================
	// 重叠事件回调（绑定到 InnerZone / OuterZone 的 OnComponentBeginOverlap / EndOverlap）
	// ========================================================================

	/** 内区开始重叠 */
	UFUNCTION()
	void On_InnerZone_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 内区结束重叠 */
	UFUNCTION()
	void On_InnerZone_EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** 外区开始重叠 */
	UFUNCTION()
	void On_OuterZone_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 外区结束重叠 */
	UFUNCTION()
	void On_OuterZone_EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// ========================================================================
	// 蓝图可覆写事件（派生蓝图 / 子类可覆写）
	// ========================================================================

	/** 交互开始事件（携带发起交互的 Pawn） */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction Target")
	void On_Interaction_Begin_Event(APawn* InteractorPawn);

	/** 交互结束事件（携带交互结果 Enum_InteractionResult 与发起交互的 Pawn） */
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction Target")
	void On_Interaction_End_Event(EInteractionResult Result, APawn* InteractorPawn);


	// Getter 函数
	UFUNCTION(BlueprintPure, Category = "Interaction")
	USceneComponent* GetMarkerTargetComponent() const { return MarkerTargetComponent; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FVector GetMarkerOffset() const { return MarkerOffset; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetOwnerReference() const { return OwnerReference; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool GetUseCustomKeys() const { return UseCustomKeys; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	EInteractionType GetInteractionType() const { return InteractionType; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetHoldSeconds() const { return HoldSeconds;}

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetRepeatCooldown() const {return RepeatCooldown;}

	UFUNCTION(BlueprintPure, Category = "Interaction")
	int32 GetRepeatCount() const { return RepeatCount; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool GetCooldownEnabled() const { return CooldownEnabled; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	EInteractionFinishMethod GetInteractionFinishMethod() const { return FinishMethod; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	USphereComponent* GetOuterZone() const { return OuterZone; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	USphereComponent* GetInnerZone() const { return InnerZone; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool GetInteractionEnabled() const { return InteractionEnabled; }

	UFUNCTION()
	void SetInteractionEnabled(bool bEnabled) { InteractionEnabled = bEnabled; }

	//UFUNCTION(BlueprintPure, Category = "Interaction")
	//AActor* GetOwnerReference() const { return OwnerReference; }

	// 获取自定义按键（返回 const 引用，避免拷贝）
	UFUNCTION(BlueprintPure, Category = "Interaction")
	const TArray<FKey>& GetCustomKeys() const { return CustomKeys; }

	// 获取已分配的交互者（返回 const 引用，避免拷贝）
	UFUNCTION(BlueprintPure, Category = "Interaction")
	const TArray<AController*>& GetAssignedInteractors() const { return AssignedInteractors; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetLastInteractedTime() const { return LastInteractedTime; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	float GetReactivationDuration() const { return ReactivationDuration; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FLinearColor GetHighlightColor() const { return HighlightColor; }


	// 触发交互结束事件（供外部调用）
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void NotifyInteractionEnd(EInteractionResult Result, APawn* InstigatorPawn)
	{
		OnInteractionEnd.Broadcast(Result, InstigatorPawn);
	}
	// 触发交互重新激活事件（供外部调用）
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void NotifyInteractionReactivated(APawn* InstigatorPawn)
	{
		OnInteractionReactivated.Broadcast(InstigatorPawn);
	}

protected:
	// ========================================================================
	// 生命周期（对应蓝图事件 ReceiveBeginPlay）
	// ========================================================================

	/** 对应蓝图 ReceiveBeginPlay */
	virtual void BeginPlay() override;

	

	// ========================================================================
	// 成员变量（分类对应蓝图 Category）
	// ========================================================================

	// ---------- Interaction Target | Default（配置项） ----------

	/** 内区球形触发组件（进入即开始交互） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	USphereComponent* InnerZone = nullptr;

	/** 外区球形触发组件（进入即显示 marker） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	USphereComponent* OuterZone = nullptr;

	/** 屏幕半径百分比（0~1） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ScreenRadiusPercent = 0.5f;

	/** 控件边距 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FMargin WidgetMargin;

	/** 所有者 Actor 引用（运行时缓存） */
	UPROPERTY(BlueprintReadOnly, Category = "Interaction Target")
	AActor* OwnerReference = nullptr;

	/** 标记组件名称 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FString MarkerComponentName;

	/** 交互区要附加到的组件名 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FString InteractionZone_ComponentToAttach;

	/** 标记相对偏移 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FVector MarkerOffset = FVector::ZeroVector;

	/** 需要高亮的组件名列表 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	TArray<FString> ComponentsToHighlight;

	/** 高亮颜色 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FLinearColor HighlightColor = FLinearColor::White;

	/** 目标图标颜色 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FLinearColor Target_IconColor = FLinearColor::White;

	/** 兴趣点（POI）图标颜色 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FLinearColor POI_IconColor = FLinearColor::White;

	/** 交互图标背景色 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FLinearColor Interaction_Icon_Background_Color = FLinearColor::White;

	/** 交互图标前景色 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FLinearColor Interaction_Icon_Foreground_Color = FLinearColor::White;

	/** 交互文本（键为交互状态 Enum_InteractionState，值为显示文本） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	TMap<EInteractionState, FText> InteractionText;

	/** 内区半径 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (ClampMin = "0.0"))
	float InnerZoneRadius = 100.f;

	/** 外区范围（半长/半径） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (ClampMin = "0.0"))
	float OuterZoneExtent = 300.f;

	/** 按住持续时间（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (ClampMin = "0.0"))
	float HoldSeconds = 0.f;

	/** 重新激活持续时间（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (ClampMin = "0.0"))
	float ReactivationDuration = 0.f;

	/** 重复交互冷却（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (ClampMin = "0.0"))
	float RepeatCooldown = 0.f;

	/** 上次交互时间（运行时） */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction Target")
	float LastInteractedTime = 0.f;

	/** 是否显示进度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool DisplayProgress = true;

	/** 是否显示三角指示 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool DisplayTriangle = true;

	/** POI 图标是否闪烁 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool FlashPOI_Icon = false;

	/** 目标图标是否闪烁 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool Flash_TargetIcon = false;

	/** 交互图标是否旋转 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool Rotate_InteractionIcon = false;

	/** 是否显示交互按键 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool DisplayInteractionKey = true;

	/** 是否使用自定义按键 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target", meta = (AllowPrivateAccess = "true"))
	bool UseCustomKeys = false;

	/** 是否启用冷却 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool CooldownEnabled = false;

	/** 是否启用调试 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	bool EnableDebug = false;

	/** 目标图标（Struct_IconSelector） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FStructIconSelector Target_Icon;

	/** 兴趣点图标（Struct_IconSelector） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FStructIconSelector POI_Icon;

	/** 交互图标背景（Struct_IconSelector） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FStructIconSelector Interaction_Icon_Background;

	/** 交互图标前景（Struct_IconSelector） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	FStructIconSelector Interaction_Icon_Foreground;

	/** 自定义交互按键 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	TArray<FKey> CustomKeys;

	/** 交互类型（Enum_InteractionType） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	EInteractionType InteractionType = EInteractionType::Tap;

	/** 网络处理方式（Enum_InteractionNetworkHandleMethod） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	EInteractionNetworkHandleMethod NetworkHandleMethod = EInteractionNetworkHandleMethod::KeepEnabled;

	/** 交互结束方式（Enum_InteractionFinishMethod） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	EInteractionFinishMethod FinishMethod = EInteractionFinishMethod::DestroyOnCompleted;

	/** 重复次数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction Target")
	int32 RepeatCount = 0;

	// ---------- Interaction Target | Data（运行时状态） ----------

	/** 是否启用交互（运行时） */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction Target|Data")
	bool InteractionEnabled = true;

	/** 已高亮的组件列表（运行时） */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction Target|Data")
	TArray<UPrimitiveComponent*> HighlightedComponents;

	/** 标记目标组件（运行时，供 marker 定位） */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction Target|Data")
	USceneComponent* MarkerTargetComponent = nullptr;

	
	/** 已分配的交互器控制器列表（运行时） */
	UPROPERTY(BlueprintReadWrite, Category = "Interaction Target|Data", meta = (AllowPrivateAccess = "true"))
	TArray<AController*> AssignedInteractors;

	//辅助函数
	UManager_Interactor* GetInteractorManager(AController* Controller);
};
