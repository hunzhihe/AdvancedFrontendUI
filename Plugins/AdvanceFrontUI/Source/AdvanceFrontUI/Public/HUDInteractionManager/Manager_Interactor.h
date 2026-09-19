// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"		// FKey
#include "HUDInteractionManager/InteractionEnums.h"	// 交互系统枚举
#include "Manager_Interactor.generated.h"

// ============================================================================
// 蓝图交互管理器（Interactor）的 C++ 声明骨架
// 原蓝图：/Game/InteractionManager/Blueprints/Components/Manager_Interactor
// 说明：
//   - 本类负责「发现/筛选最佳可交互目标、交互按键、交互流程、marker 控件池」等逻辑。
//   - 网络同步采用 RPC（Server_* / Client_*），仅 OwnerController 属性走复制。
//   - 蓝图里引用到的枚举已转换为 UENUM，见 InteractionEnums.h：
//       Enum_InteractionType             -> EInteractionType
//       Enum_InteractionState            -> EInteractionState（InteractionText 的 Map 键）
//       Enum_InteractionNetworkHandleMethod -> EInteractionNetworkHandleMethod
//       Enum_InteractionFinishMethod     -> EInteractionFinishMethod
//       Enum_InteractionResult           -> EInteractionResult（函数参数 Result）
//   - 蓝图里的 float/double 混用（UE5 蓝图 float 引脚默认即 double），此处统一为 float。
// ============================================================================

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionUpdated, UManager_InteractionTarget*, InteractionTarget, double, Alpha);
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInteractionUpdatedWithRepeat, UManager_InteractionTarget*, InteractionTarget, double, Alpha, int32, Repeated);
// 前向声明
class UInputAction;
class UPostProcessComponent;
class UUserWidget;
class UMaterialInstanceDynamic;
class UManager_InteractionTarget;
class APawn;
class UInteractionWidget;
class APlayerController;

/**
 * 交互管理器组件（Interactor）
 * 挂载在玩家身上，负责从 InteractionTargets 中筛选最佳可交互目标，
 * 维护交互按键、交互进度，并通过 RPC 与服务器同步。
 */
UCLASS(Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVANCEFRONTUI_API UManager_Interactor : public UActorComponent
{
	GENERATED_BODY()

public:
	UManager_Interactor();


	//UPROPERTY(BlueprintAssignable, Category = "Interaction")
	//FOnInteractionUpdated OnInteractionUpdated;

	//UPROPERTY(BlueprintAssignable, Category = "Interaction")
	//FOnInteractionUpdatedWithRepeat OnInteractionUpdatedWithRepeat;

	// ========================================================================
	// 构造 / 初始化（蓝图中由 BeginPlay 依次调用）
	// ========================================================================

	/** 构造玩家要素：缓存 OwnerController、PlayerCameraManager 等引用 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Construct_Player_Essentials();

	/** 构造后期处理组件（用于目标高亮描边） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Construct_PostProcess_Component();

	/** 构造池化的交互标记控件（WB_InteractionTarget） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Consturct_Pooled_Marker_Widgets();

	/** 调试函数（仅在 Debug 开关打开时输出日志） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Debug_Functions();

	// 函数声明
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager|Input")
	void Receive_AnyKey(const FKey& Key);

	// ========================================================================
	// 目标查找与筛选
	// ========================================================================

	/** 从控件池中查找一个空闲（未被占用）的标记控件 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	UInteractionWidget* Find_Empty_Widget();

	/** 根据交互目标查找其对应的标记控件 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	UInteractionWidget* Find_Widget_By_InteractionTarget(UManager_InteractionTarget* InteractionTarget);

	/**
	 * 筛选最佳可交互目标。
	 * 算法：遍历 InteractionTargets，跳过不可交互目标，计算「目标标记原点」相对相机前向的点积，
	 * 取点积最大（最靠屏幕中心）且大于阈值 0.5 的目标。返回可能为空。
	 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	UManager_InteractionTarget* Find_Best_Interactable();

	/** 判断目标是否可交互（原蓝图函数名 IsInteractable?） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	bool IsInteractable(UManager_InteractionTarget* ItemToFind);

	/** 更新当前最佳交互目标（刷新 marker 高亮、按键提示等） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Update_Best_Interactable(UManager_InteractionTarget* NewTarget);

	// ========================================================================
	// 交互按键
	// ========================================================================

	/** 根据当前输入设备（键鼠/手柄）刷新 InteractionKeys */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Update_InteractionKeys();

	/**
	 * 获取当前可用的交互按键列表。
	 * @param OutSuccess 输出参数：是否存在有效交互按键
	 * @return 交互按键数组（无则返回空数组）
	 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Get_InteractionKeys(bool& Success, TArray<FKey>& ReturnValue);

	// ========================================================================
	// 交互流程
	// ========================================================================

	/** 尝试发起/进行交互（按住、连击、冷却等逻辑入口） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Try_Take_Interaction();

	/**
	 * 应用交互结束方式。
	 * @param Result 交互结果（对应蓝图枚举 Enum_InteractionResult）
	 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Apply_Finish_Method(UManager_InteractionTarget* InteractionTarget, EInteractionResult Result);

	/** 将目标加入待处理（Pending）列表 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Add_To_Pending_Targets(UManager_InteractionTarget* InteractionTarget);

	/** 将目标加入已停用（Deactivated）列表 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Add_To_Deactivated_Targets(UManager_InteractionTarget* InteractionTarget);

	/** 将目标从已停用（Deactivated）列表移除 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Remove_From_Deactivated_Targets(UManager_InteractionTarget* InteractionTarget);

	/** 定时检查待处理目标（由 PendingTarget_TimerHandle 触发） */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Check_For_Pending_Targets();

	/** 设置目标高亮（描边材质）开关 */
	UFUNCTION(BlueprintCallable, Category = "Interactor Manager")
	void Set_Target_Highlighted(UManager_InteractionTarget* InteractionTarget, bool IsHighlighted);

	// ========================================================================
	// 事件回调（绑定到 Manager_InteractionTarget 的事件委托）
	// ========================================================================

	/** 交互目标被销毁时回调（绑定 Actor OnDestroyed） */
	UFUNCTION()
	void On_InteractionTarget_Destroyed(AActor* DestroyedActor);

	/** 交互目标重新激活时回调 */
	UFUNCTION()
	void On_InteractionTarget_Reactivated(UManager_InteractionTarget* InteractionTarget);

	/** 兴趣点更新（服务器侧） */
	UFUNCTION()
	void On_PointOfInterest_Updated_ServerSide(bool Add, UManager_InteractionTarget* InteractionTarget);

	/** 兴趣点更新（客户端侧） */
	UFUNCTION()
	void On_PointOfInterest_Updated_ClientSide(bool Add, UManager_InteractionTarget* InteractionTarget);

	/** 交互目标更新（服务器侧） */
	UFUNCTION()
	void On_InteractionTarget_Updated_ServerSide(bool Add, UManager_InteractionTarget* InteractionTarget);

	/** 交互目标更新（客户端侧） */
	UFUNCTION()
	void On_InteractionTarget_Updated_ClientSide(bool Add, UManager_InteractionTarget* InteractionTarget);

	/** 新目标被选中（客户端侧） */
	UFUNCTION()
	void On_New_Target_Selected_ClientSide(UManager_InteractionTarget* NewTarget, bool IsSelected);

	///** 交互进度更新（客户端侧事件） */
	UFUNCTION(BlueprintNativeEvent, Category = "Interactor Manager")
	void On_Interaction_Updated(UManager_InteractionTarget* InteractionTarget, float Alpha, int32 InRepeated);

	// ========================================================================
	// 网络 RPC —— Server（由客户端调用，服务器执行，Reliable）
	// ========================================================================

	UFUNCTION(Server, Reliable)
	void Server_On_Interaction_Updated(UManager_InteractionTarget* InteractionTarget, float Alpha, int32 InRepeated, APawn* InteractorPawn);

	UFUNCTION(Server, Reliable)
	void Server_On_Interaction_Begin(UManager_InteractionTarget* InteractionTarget);

	UFUNCTION(Server, Reliable)
	void Server_Request_Assign_Interactor(UManager_InteractionTarget* InteractionTarget, bool Add);

	UFUNCTION(Server, Reliable)
	void Server_Update_PointOfInterests(bool Add, UManager_InteractionTarget* InteractionTarget);

	UFUNCTION(Server, Reliable)
	void Server_Update_InteractionTargets(bool Add, UManager_InteractionTarget* InteractionTarget);

	UFUNCTION(Server, Reliable)
	void Server_On_Interaction_Finished(UManager_InteractionTarget* InteractionTarget, EInteractionResult Result);

	// ========================================================================
	// 网络 RPC —— Client（由服务器调用，客户端执行，Reliable）
	// ========================================================================

	UFUNCTION(Client, Reliable)
	void Client_Check_Pressed_Key();

	UFUNCTION(Client, Reliable)
	void Client_On_Interaction_Target_Destroyed(UManager_InteractionTarget* InteractionTarget);

	UFUNCTION(Client, Reliable)
	void Client_Update_InteractionTargets(bool Add, UManager_InteractionTarget* InteractionTarget);

	UFUNCTION(Client, Reliable)
	void Client_Update_PointOfInterests(bool Add, UManager_InteractionTarget* InteractionTarget);

	UFUNCTION(Client, Reliable)
	void Client_Reset_Data();

	UFUNCTION(Client, Reliable)
	void Client_Set_New_Target(UManager_InteractionTarget* NewTarget, bool IsSelected);

	//辅助函数

	


	UFUNCTION(Client, Reliable)
	void Client_Update_Interaction_Text_Begin(UManager_InteractionTarget* Target);

	UFUNCTION(Client, Reliable)
	void Client_Set_Interaction_Percent(double Percent);

	UFUNCTION(Client, Reliable)
	void Client_Play_Completed_Animation(UManager_InteractionTarget* Target, uint8 Result);

	UFUNCTION(Client, Reliable)
	void Client_Reset_Progress();

	APawn* GetOwnerPawn() const
	{
		if (!OwnerController)
		{
			return nullptr;
		}
		return OwnerController->GetPawn();
	}

	void HandleCompleted(UManager_InteractionTarget* Target);
	void HandleCanceled(UManager_InteractionTarget* Target);



protected:
	// ========================================================================
	// 生命周期（对应蓝图事件 ReceiveBeginPlay / ReceiveTick / Receive_AnyKey）
	// ========================================================================

	/** 对应蓝图 ReceiveBeginPlay */
	virtual void BeginPlay() override;

	/** 对应蓝图 ReceiveTick */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 对应蓝图「AnyKey」输入事件；返回 true 表示已处理该按键 */
	//virtual bool InputKey(FKey Key) override;

	// ========================================================================
	// 成员变量（分类对应蓝图 Category）
	// ========================================================================

	// ---------- Interactor Manager | Main（配置项） ----------

	/** 调试开关 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Main")
	bool Debug = false;

	/** 默认控件池大小 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Main")
	int32 DefaultWidgetPoolSize = 8;

	/** 待处理目标检查间隔（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Main", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float PendingTargetCheckInterval = 0.1f;

	/** 交互输入动作（EnhancedInput） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Main")
	UInputAction* InteractionInputAction = nullptr;

	// ---------- Interactor Manager | Marker Settings（标记设置） ----------

	/** 标记控件在屏幕上的边距 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Marker Settings")
	FMargin WidgetScreenMargin;

	/** 屏幕半径百分比（0~1，用于判定目标是否在屏幕范围内） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Marker Settings", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ScreenRadiusPercent = 0.5f;

	// ---------- Interactor Manager | Data（运行时状态） ----------

	/** 所有已注册的可交互目标 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	TArray<UManager_InteractionTarget*> InteractionTargets;

	/** 兴趣点（POI）列表 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	TArray<UManager_InteractionTarget*> PointOfInterests;

	/** 待处理目标列表 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	TArray<UManager_InteractionTarget*> PendingTargets;

	/** 已停用目标列表 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	TArray<UManager_InteractionTarget*> DeactivatedTargets;

	/** 标记控件池 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	TArray<UInteractionWidget*> WidgetPool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Widget Pool")
	TSubclassOf<UInteractionWidget> MarkerWidgetClass;


	/** 拥有者控制器（唯一走属性复制的成员，服务器分配后复制给客户端） */
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Interactor Manager|Data")
	APlayerController* OwnerController = nullptr;

	/** 后期处理组件（目标高亮描边） */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	UPostProcessComponent* PostProcessComponent = nullptr;

	// 材质配置（可在蓝图中指定）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactor Manager|Post Process")
	TSoftObjectPtr<UMaterialInterface> OutlineMaterial;

	/** 当前交互标记控件 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	UInteractionWidget* CurrentInteractionMarker = nullptr;

	/** 当前最佳交互目标 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	UManager_InteractionTarget* BestInteractionTarget = nullptr;

	/** 当前可用的交互按键 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	TArray<FKey> InteractionKeys;

	/** 当前按住时长 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	float CurrentHoldTime = 0.f;

	/** 重复交互冷却 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	float RepeatCooldown = 0.f;

	/** 已重复次数 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	int32 Repeated = 0;

	/** 是否正在交互 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	bool IsInteracting = false;

	/** 按键是否刚按下 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	bool KeyJustPressed = false;

	/** 是否为手柄输入 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	bool IsGamepad = false;

	/** 最后按下的键 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	FKey LastPressedKey;

	/** 更新按键的定时器句柄 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	FTimerHandle BeginUpdateKeys_TimerHandle;

	/** 待处理目标检查的定时器句柄 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	FTimerHandle PendingTarget_TimerHandle;

	/** 描边动态材质实例 */
	UPROPERTY(BlueprintReadWrite, Category = "Interactor Manager|Data")
	UMaterialInstanceDynamic* Outline_DynamicMaterial = nullptr;

	
};
