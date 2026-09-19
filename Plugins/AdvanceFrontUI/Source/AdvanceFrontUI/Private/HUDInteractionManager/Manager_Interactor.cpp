// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDInteractionManager/Manager_Interactor.h"
#include "HUDInteractionManager/Manager_InteractionTarget.h"
#include "HUDInteractionManager/WidgetWithTargetInterface.h"
#include "HUDInteractionManager/InteractionWidget.h"
#include "HUDInteractionManager/InteractionEnums.h"	
#include "EnhancedInputSubsystems.h"
#include "Components/PostProcessComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetInputLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"

// ============================================================================
// 构造函数
// ============================================================================
UManager_Interactor::UManager_Interactor()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 本组件需走网络复制（仅 OwnerController 属性）
	SetIsReplicatedByDefault(true);
}

// ============================================================================
// 属性复制
// ============================================================================
void UManager_Interactor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 仅 OwnerController 走属性复制，其余状态通过 RPC 同步
	DOREPLIFETIME(UManager_Interactor, OwnerController);
}

// ============================================================================
// 生命周期（对应蓝图 ReceiveBeginPlay / ReceiveTick / Receive_AnyKey）
// ============================================================================
void UManager_Interactor::BeginPlay()
{
	Super::BeginPlay();

	// TODO: 实现初始化逻辑（蓝图 BeginPlay 中的连线）
}

void UManager_Interactor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// TODO: 实现每帧逻辑（蓝图 Tick 中的连线）
}

//bool UManager_Interactor::InputKey(FKey Key)
//{
	
	// TODO: 实现「AnyKey」输入事件逻辑
	//return Super::InputKey(Key);
//}

// ============================================================================
// 构造 / 初始化
// ============================================================================
void UManager_Interactor::Construct_Player_Essentials()
{
	// TODO: 缓存 OwnerController、PlayerCameraManager 等引用


	// 1. 获取 Owner Actor 并尝试转换为 PlayerController
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		OwnerController = Cast<APlayerController>(OwnerActor);
		// 如果转换失败，OwnerController 为 nullptr，但蓝图并未处理失败情况，直接继续
	}

	// 2. 构造后期处理组件（调用自定义函数）
	Construct_PostProcess_Component();

	// 3. 设置延迟定时器，0.2 秒后调用 Update_InteractionKeys（仅一次）
	if (GetWorld())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UManager_Interactor::Update_InteractionKeys);
		GetWorld()->GetTimerManager().SetTimer(
			BeginUpdateKeys_TimerHandle,
			TimerDelegate,
			0.2f,
			false,  // bLooping = false
			0.0f    // InitialStartDelay 默认 0
		);
	}
}

void UManager_Interactor::Construct_PostProcess_Component()
{
	// TODO: 创建后期处理组件并初始化描边材质

	// 1. 获取 OwnerController 控制的 Pawn
	if (!OwnerController)
	{
		return;
	}
	APawn* Pawn = OwnerController->GetPawn();
	if (!Pawn)
	{
		return;
	}

	// 2. 创建并添加 PostProcessComponent
	UPostProcessComponent* NewPPComp = NewObject<UPostProcessComponent>(Pawn);
	if (!NewPPComp)
	{
		return;
	}

	NewPPComp->RegisterComponent();
	Pawn->AddInstanceComponent(NewPPComp);

	USceneComponent* Root = Pawn->GetRootComponent();
	if (Root)
	{
		NewPPComp->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
	}

	PostProcessComponent = NewPPComp;

	// 3. 创建动态材质实例（使用可配置参数）
	UMaterialInterface* Material = nullptr;

	// 优先使用配置的材质
	if (!OutlineMaterial.IsNull())
	{
		Material = OutlineMaterial.LoadSynchronous();
	}

	// 如果未配置或加载失败，使用默认路径（向后兼容）
	if (!Material)
	{
		Material = LoadObject<UMaterialInterface>(
			nullptr,
			TEXT("/Game/InteractionManager/Environment/Materials/PostProcess/M_OutlineMaterial.M_OutlineMaterial")
		);
		if (!Material)
		{
			UE_LOG(LogTemp, Warning, TEXT("Construct_PostProcess_Component: Failed to load Outline Material"));
			return;
		}
	}

	UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(Material, this);
	if (!MID)
	{
		UE_LOG(LogTemp, Warning, TEXT("Construct_PostProcess_Component: Failed to create Dynamic Material Instance"));
		return;
	}
	Outline_DynamicMaterial = MID;

	// 4. 将动态材质添加到后处理组件的 Settings.WeightedBlendables
	if (PostProcessComponent && Outline_DynamicMaterial)
	{
		FWeightedBlendable Blendable;
		Blendable.Object = Outline_DynamicMaterial;
		Blendable.Weight = 1.0f;

		PostProcessComponent->Settings.WeightedBlendables.Array.Add(Blendable);
	}

}

void UManager_Interactor::Consturct_Pooled_Marker_Widgets()
{
	// TODO: 按 DefaultWidgetPoolSize 创建并池化标记控件
	// 1. 检查是否为本地玩家控制器
	if (!OwnerController || !OwnerController->IsLocalPlayerController())
	{
		return;
	}

	// 2. 检查 Widget 类是否已设置
	if (!MarkerWidgetClass)
	{
		// 如果未设置，尝试加载默认路径（向后兼容）
		MarkerWidgetClass = LoadClass<UInteractionWidget>(nullptr, TEXT("/Game/InteractionManager/UserInterface/WB_InteractionTarget.WB_InteractionTarget_C"));
		if (!MarkerWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("Consturct_Pooled_Marker_Widgets: MarkerWidgetClass is not set and default path failed to load"));
			return;
		}
	}

	// 3. 循环创建 Widget
	for (int32 i = 0; i < DefaultWidgetPoolSize; ++i)
	{
		UInteractionWidget* NewWidget = CreateWidget<UInteractionWidget>(OwnerController, MarkerWidgetClass);
		if (!NewWidget)
		{
			continue;
		}

		WidgetPool.AddUnique(NewWidget);
		NewWidget->AddToPlayerScreen(0);
	}
}

void UManager_Interactor::Debug_Functions()
{
	// TODO: 输出调试日志

	// 1. 检查调试开关
	if (!Debug)
	{
		return;
	}

	// 2. 检查是否具有服务器权限（对应 Switch Has Authority 宏）
	//    GetOwner()->HasAuthority() 对于 ActorComponent 有效
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !OwnerActor->HasAuthority())
	{
		return;
	}

	// 3. 获取各数组长度
	int32 InteractionTargetsCount = InteractionTargets.Num();
	int32 PointOfInterestsCount = PointOfInterests.Num();
	int32 PendingTargetsCount = PendingTargets.Num();

	// 4. 获取 CurrentInteractionMarker 的显示名称
	FString MarkerDisplayName;
	if (CurrentInteractionMarker)
	{
		MarkerDisplayName = UKismetSystemLibrary::GetDisplayName(CurrentInteractionMarker);
	}
	else
	{
		MarkerDisplayName = TEXT("None");
	}

	// 5. 构建调试字符串
	FString DebugMessage = FString::Printf(
		TEXT("Interaction Targets: %d\r\nPoint Of Interests: %d\r\nPending Targets: %d\r\nMarker: %s"),
		InteractionTargetsCount,
		PointOfInterestsCount,
		PendingTargetsCount,
		*MarkerDisplayName
	);

	// 6. 打印到屏幕（蓝图中的 PrintString 默认打印到屏幕，持续时间 0 表示永久）
	UKismetSystemLibrary::PrintString(
		this,
		DebugMessage,
		true,   // bPrintToScreen
		false,  // bPrintToLog
		FLinearColor(0.0f, 0.66f, 1.0f, 1.0f),  // 蓝色
		0.0f    // Duration = 0 表示永久显示（直到下次调用覆盖）
	);

}

void UManager_Interactor::Receive_AnyKey(const FKey& Key)
{
	// 判断按键是否为游戏手柄按键
	IsGamepad = UKismetInputLibrary::Key_IsGamepadKey(Key);
}

// ============================================================================
// 目标查找与筛选
// ============================================================================
UInteractionWidget* UManager_Interactor::Find_Empty_Widget()
{
	// TODO: 从 WidgetPool 中返回一个空闲控件
	for (UInteractionWidget* Widget : WidgetPool)
	{
		if (!Widget)
		{
			continue;
		}

		// 转换为接口
		IWidgetWithTargetInterface* TargetInterface = Cast<IWidgetWithTargetInterface>(Widget);
		if (!TargetInterface)
		{
			// 如果 Widget 没有实现接口，跳过
			continue;
		}

		// 调用接口方法获取 InteractionTarget
		UManager_InteractionTarget* Target = TargetInterface->GetInteractionTarget();
		if (Target == nullptr)
		{
			// 找到空的 Widget
			return Widget;
		}
	}
	return nullptr;
}

UInteractionWidget* UManager_Interactor::Find_Widget_By_InteractionTarget(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 查找目标对应的标记控件
	for (UInteractionWidget* Widget : WidgetPool)
	{
		if (!Widget)
		{
			continue;
		}

		// 转换为接口
		IWidgetWithTargetInterface* TargetInterface = Cast<IWidgetWithTargetInterface>(Widget);
		if (!TargetInterface)
		{
			continue;
		}

		// 获取 Widget 关联的 InteractionTarget
		UManager_InteractionTarget* WidgetTarget = TargetInterface->GetInteractionTarget();
		if (WidgetTarget == InteractionTarget)
		{
			// 找到匹配的 Widget
			return Widget;
		}
	}
	return nullptr;
}

UManager_InteractionTarget* UManager_Interactor::Find_Best_Interactable()
{
	// TODO: 点积视锥筛选 + 取点积最大目标（阈值 0.5）
	// 局部变量：缓存当前遍历的目标、最终选中的目标、位置和点积值
	UManager_InteractionTarget* LocCachedInteractable = nullptr;
	UManager_InteractionTarget* LocTargetInteractable = nullptr;
	FVector LocActorOrigin = FVector::ZeroVector;
	double LocDotProduct = 0.0;

	// 遍历所有交互目标
	for (UManager_InteractionTarget* Target : InteractionTargets)
	{
		LocCachedInteractable = Target;

		// 检查目标是否可交互（不为空、已启用、不在挂起或停用列表中）
		if (!IsInteractable(LocCachedInteractable))
		{
			continue;
		}

		// 获取目标的标记组件位置，并加上旋转后的偏移量
		USceneComponent* MarkerComp = LocCachedInteractable->GetMarkerTargetComponent();
		if (!MarkerComp)
		{
			continue;
		}

		FVector MarkerLocation = MarkerComp->GetComponentLocation();
		FRotator MarkerRotation = MarkerComp->GetComponentRotation();
		FVector OffsetRotated = UKismetMathLibrary::GreaterGreater_VectorRotator(
			LocCachedInteractable->GetMarkerOffset(),
			MarkerRotation
		);
		LocActorOrigin = MarkerLocation + OffsetRotated;

		// 获取玩家相机位置和朝向
		if (!OwnerController)
		{
			continue;
		}
		APlayerCameraManager* CameraManager = OwnerController->PlayerCameraManager;
		if (!CameraManager)
		{
			continue;
		}

		FVector CameraLocation = CameraManager->GetCameraLocation();
		FRotator CameraRotation = CameraManager->GetCameraRotation();
		FVector CameraForward = UKismetMathLibrary::GetForwardVector(CameraRotation);

		// 计算从相机到目标的方向向量并归一化
		FVector DirToTarget = LocActorOrigin - CameraLocation;
		FVector DirToTargetNormalized = UKismetMathLibrary::Normal(DirToTarget, 0.0001f);

		// 计算点积
		double DotValue = UKismetMathLibrary::Dot_VectorVector(DirToTargetNormalized, CameraForward);

		// 如果点积大于阈值 0.5，且大于当前保存的最佳值，则更新
		if (DotValue > 0.5 && DotValue > LocDotProduct)
		{
			LocDotProduct = DotValue;
			LocTargetInteractable = LocCachedInteractable;
		}
	}

	// 如果找到了候选目标，进一步检查其 Owner 是否最近被渲染
	if (LocTargetInteractable)
	{
		AActor* OwnerActor = LocTargetInteractable->GetOwnerReference();
		if (OwnerActor && OwnerActor->WasRecentlyRendered(0.2f))
		{
			return LocTargetInteractable;
		}
		else
		{
			// 如果最近的被渲染目标无效，返回空（即不使用不可见的目标）
			// 注意：蓝图中的逻辑是，如果 WasRecentlyRendered 返回 false，则不返回任何目标
			return nullptr;
		}
	}

	// 没有找到合适的目标
	return nullptr;
}

bool UManager_Interactor::IsInteractable(UManager_InteractionTarget* ItemToFind)
{
	// TODO: 判断目标是否可交互
	// 1. 空指针检查
	if (!ItemToFind)
	{
		return false;
	}

	// 2. 检查目标自身是否启用（调用目标上的 IsInteractionEnabled?）
	if (!ItemToFind->IsInteractionEnabled())
	{
		return false;
	}

	// 3. 检查目标是否在 PendingTargets 或 DeactivatedTargets 中
	if (PendingTargets.Contains(ItemToFind))
	{
		return false;
	}
	if (DeactivatedTargets.Contains(ItemToFind))
	{
		return false;
	}

	// 所有条件满足，可交互
	return true;
}

void UManager_Interactor::Update_Best_Interactable(UManager_InteractionTarget* NewTarget)
{
	// TODO: 刷新最佳交互目标及相关 marker 状态
	// 1. 检查 NewTarget 是否有效
	if (NewTarget)
	{
		// 2. 检查 NewTarget 是否与当前 BestInteractionTarget 不同
		if (NewTarget != BestInteractionTarget)
		{
			// 3. 处理旧目标（如果存在）
			if (BestInteractionTarget)
			{
				// 触发旧目标的 OnInteractionEnd 委托
				BestInteractionTarget->NotifyInteractionEnd(EInteractionResult::Canceled, GetOwnerPawn());
				// 重置客户端数据
				Client_Reset_Data();
			}

			// 4. 设置新目标为 BestInteractionTarget
			BestInteractionTarget = NewTarget;

			// 5. 根据 IsInteracting 状态决定后续操作
			if (IsInteracting)
			{
				// 正在交互中：触发旧目标（实际上还是当前目标）的 OnInteractionEnd 委托
				// 注意：这里与蓝图逻辑一致，IsInteracting=true 时触发 End 委托并重置
				BestInteractionTarget->NotifyInteractionEnd(EInteractionResult::Canceled, GetOwnerPawn());
				Client_Reset_Data();
			}
			else
			{
				// 不在交互中：直接设置客户端新目标（选中状态）
				Client_Set_New_Target(BestInteractionTarget, true);
			}
		}
		else
		{
			// NewTarget 与 BestInteractionTarget 相同
			if (IsInteracting)
			{
				// 正在交互中：触发 End 委托并重置
				BestInteractionTarget->NotifyInteractionEnd(EInteractionResult::Canceled, GetOwnerPawn());
				Client_Reset_Data();
			}
			// 如果不在交互中，不做任何操作
		}
	}
	else
	{
		// NewTarget 无效
		if (BestInteractionTarget)
		{
			// 有旧目标：触发 End 委托并重置
			BestInteractionTarget->NotifyInteractionEnd(EInteractionResult::Canceled, GetOwnerPawn());
			Client_Reset_Data();
		}
		// 如果 BestInteractionTarget 也为空，不做任何操作
	}
}

// ============================================================================
// 交互按键
// ============================================================================
void UManager_Interactor::Update_InteractionKeys()
{
	// TODO: 按当前输入设备刷新 InteractionKeys
	// 1. 检查是否为本地玩家控制器
	if (!OwnerController || !OwnerController->IsLocalPlayerController())
	{
		return;
	}

	// 2. 获取 Enhanced Input 本地玩家子系统
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(OwnerController->GetLocalPlayer());
	if (!InputSubsystem)
	{
		return;
	}

	// 3. 检查 InteractionInputAction 是否有效
	if (!InteractionInputAction)
	{
		return;
	}

	// 4. 查询按键映射
	TArray<FKey> MappedKeys = InputSubsystem->QueryKeysMappedToAction(InteractionInputAction);
	InteractionKeys = MappedKeys;

	// 5. 清除定时器句柄（BeginUpdateKeys_TimerHandle）
	//    注意：K2_ClearAndInvalidateTimerHandle 会清除并使句柄无效
	//    UKismetSystemLibrary::K2_ClearAndInvalidateTimerHandle(this, BeginUpdateKeys_TimerHandle);
	//    由于定时器可能由其他地方管理，这里不强制清除，但蓝图中有此操作，建议保留

	// 如果需要清除定时器，使用以下代码
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(BeginUpdateKeys_TimerHandle);
		// 注意：ClearTimer 不会自动重置 Handle 为无效，如果需要可以手动调用 Invalidate
		BeginUpdateKeys_TimerHandle.Invalidate();
	}
}

void UManager_Interactor::Get_InteractionKeys(bool& Success, TArray<FKey>& ReturnValue)
{
	// TODO: 返回有效交互按键，并设置 Success
	// 检查最佳交互目标是否存在且使用自定义按键
	if (BestInteractionTarget && BestInteractionTarget->GetUseCustomKeys())
	{
		// 使用自定义键
		ReturnValue = BestInteractionTarget->GetCustomKeys();
		Success = ReturnValue.Num() > 0;
	}
	else
	{
		// 使用默认按键
		ReturnValue = InteractionKeys;
		Success = ReturnValue.Num() > 0;
	}
}

// ============================================================================
// 交互流程
// ============================================================================
void UManager_Interactor::Try_Take_Interaction()
{
	// TODO: 按住/连击/冷却等交互主流程入口
	// 1. 获取交互按键列表
	bool bKeySuccess = false;
	TArray<FKey> LocalInteractionKeys;
	Get_InteractionKeys(bKeySuccess, LocalInteractionKeys);

	// 如果按键获取失败，直接返回
	if (!bKeySuccess)
	{
		return;
	}

	// 2. 遍历所有按键，检测输入
	for (const FKey& Key : LocalInteractionKeys)
	{
		if (!OwnerController)
		{
			continue;
		}

		// 检查按键是否刚刚按下
		bool bKeyJustPressed = OwnerController->WasInputKeyJustPressed(Key);
		if (bKeyJustPressed)
		{
			// 缓存按下的按键
			LastPressedKey = Key;
			KeyJustPressed = true;

			// 3. 根据交互类型分支处理
			if (!BestInteractionTarget)
			{
				continue;
			}

			EInteractionType InteractType = BestInteractionTarget->GetInteractionType();

			if (InteractType == EInteractionType::Tap)
			{
				// === Tap 交互 ===
				// 直接触发开始和完成
				Server_On_Interaction_Begin(BestInteractionTarget);

				// 更新 UI 文本（立即显示）
				if (IWidgetWithTargetInterface* WidgetInterface = Cast<IWidgetWithTargetInterface>(CurrentInteractionMarker))
				{
					// 调用 Update_Interaction_Text 的蓝图实现
					// 由于是蓝图函数，这里用蓝图实现的 RPC 替代
					Client_Update_Interaction_Text_Begin(BestInteractionTarget);
				}

				// 播放完成动画
				if (IWidgetWithTargetInterface* WidgetInterface = Cast<IWidgetWithTargetInterface>(CurrentInteractionMarker))
				{
					// 调用 Play_Interaction_Completed_Animation
					Client_Play_Completed_Animation(BestInteractionTarget, 0); // Result = Completed
				}

				Server_On_Interaction_Finished(BestInteractionTarget, EInteractionResult::Completed); // Result = Completed
			}
			else if (InteractType == EInteractionType::Hold)
			{
				// === Hold 交互 ===
				// 检查是否已经按住（KeyJustPressed 为 true）
				if (KeyJustPressed)
				{
					// 触发开始事件（仅在第一次按下时触发）
					Server_On_Interaction_Begin(BestInteractionTarget);

					// 更新 UI 文本（立即显示）
					Client_Update_Interaction_Text_Begin(BestInteractionTarget);

					// 重置累计时间
					CurrentHoldTime = 0.0;
				}

				// 持续检测按键是否被按住
				bool bKeyDown = OwnerController->IsInputKeyDown(Key);
				if (bKeyDown)
				{
					// 累加按住时间
					float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
					CurrentHoldTime += DeltaTime;

					// 获取目标的 HoldSeconds 和 CooldownEnabled
					float HoldSeconds = BestInteractionTarget->GetHoldSeconds();
					bool bCooldownEnabled = BestInteractionTarget->GetCooldownEnabled();

					// 计算进度百分比（归一化到 0-1）
					double Progress = UKismetMathLibrary::NormalizeToRange(CurrentHoldTime, 0.0, HoldSeconds);
					Progress = UKismetMathLibrary::FMin(Progress, 1.0);

					// 更新 UI 进度
					Client_Set_Interaction_Percent(Progress);

					// 触发更新事件
					On_Interaction_Updated(BestInteractionTarget, Progress, Repeated);

					// 如果进度达到 100%，完成交互
					if (Progress >= 1.0)
					{
						// 播放完成动画
						Client_Play_Completed_Animation(BestInteractionTarget, 0); // Completed
						Server_On_Interaction_Finished(BestInteractionTarget, EInteractionResult::Completed); // Completed
						KeyJustPressed = false; // 重置状态
					}
				}
				else
				{
					// 按键已释放，检查是否已交互完成
					bool bCancelOnRelease = BestInteractionTarget->Cancel_On_Release();
					if (CurrentHoldTime > 0.0 && CurrentHoldTime < BestInteractionTarget->GetHoldSeconds())
					{
						if (bCancelOnRelease)
						{
							// 取消交互
							Client_Reset_Progress();
							Client_Play_Completed_Animation(BestInteractionTarget, 1); // Canceled
							Server_On_Interaction_Finished(BestInteractionTarget, EInteractionResult::Canceled); // Canceled
						}
						else
						{
							// 不取消，重置进度但保留目标
							Client_Reset_Progress();
						}
					}
					KeyJustPressed = false;
				}
			}
			else if (InteractType == EInteractionType::Repeat)
			{
				// === Repeat 交互 ===
				// 与 Hold 类似，但增加重复计数
				if (KeyJustPressed)
				{
					Server_On_Interaction_Begin(BestInteractionTarget);
					Client_Update_Interaction_Text_Begin(BestInteractionTarget);
					Repeated = 0;
					RepeatCooldown = 0.0;
				}

				bool bKeyDown = OwnerController->IsInputKeyDown(Key);
				if (bKeyDown)
				{
					float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

					// 如果冷却时间已到，触发重复事件
					if (RepeatCooldown <= 0.0)
					{
						// 增加重复计数
						Repeated++;

						// 获取目标的 RepeatCount
						int32 RepeatCount = BestInteractionTarget->GetRepeatCount();

						// 计算进度
						double Progress = UKismetMathLibrary::NormalizeToRange((double)Repeated, 0.0, (double)RepeatCount);
						Progress = UKismetMathLibrary::FMin(Progress, 1.0);

						// 更新 UI
						Client_Set_Interaction_Percent(Progress);
						//OnInteractionUpdatedWithRepeat.Broadcast(BestInteractionTarget, Progress, Repeated);

						// 触发更新事件（用于外部逻辑）
						On_Interaction_Updated(BestInteractionTarget, Progress,Repeated);

						// 检查是否达到目标
						if (Repeated >= RepeatCount)
						{
							Client_Play_Completed_Animation(BestInteractionTarget, 0); // Completed
							Server_On_Interaction_Finished(BestInteractionTarget, EInteractionResult::Completed); // Completed
							KeyJustPressed = false;
						}

						// 重置冷却时间
						RepeatCooldown = BestInteractionTarget->GetRepeatCooldown();
					}
					else
					{
						// 减少冷却时间
						RepeatCooldown = UKismetMathLibrary::FMax(RepeatCooldown - DeltaTime, 0.0);
					}
				}
				else
				{
					// 按键释放
					if (Repeated > 0 && Repeated < BestInteractionTarget->GetRepeatCount())
					{
						// 未完成，触发取消
						Client_Reset_Progress();
						Client_Play_Completed_Animation(BestInteractionTarget, 1); // Canceled
						Server_On_Interaction_Finished(BestInteractionTarget, EInteractionResult::Canceled); // Canceled
					}
					KeyJustPressed = false;
				}
			}
		}
	}
}

void UManager_Interactor::Apply_Finish_Method(UManager_InteractionTarget* InteractionTarget, EInteractionResult Result)
{
	// TODO: 根据 FinishMethod 与 Result 应用交互结束逻辑

	if (!InteractionTarget)
	{
		return;
	}

	IsInteracting = false;
	Client_Reset_Data();

	APawn* InteractorPawn = OwnerController ? OwnerController->GetPawn() : nullptr;
	InteractionTarget->NotifyInteractionEnd(Result, InteractorPawn);

	// 根据 Result 选择处理逻辑
	if (Result == EInteractionResult::Completed)
	{
		HandleCompleted(InteractionTarget);
	}
	else // Canceled
	{
		HandleCanceled(InteractionTarget);
	}

}

// 辅助函数处理 Completed
void UManager_Interactor::HandleCompleted(UManager_InteractionTarget* Target)
{
	switch (Target->GetInteractionFinishMethod())
	{
		// 销毁
	case EInteractionFinishMethod::DestroyOnCompleted:
	case EInteractionFinishMethod::DestroyOnCompletedOrCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DestroyOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCanceled_DestroyOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCompleted_DestroyOnCanceled:
		if (Target->GetOwnerReference())
		{
			Target->GetOwnerReference()->K2_DestroyActor();
		}
		break;

		// 重新激活（添加到待处理）
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompletedOrCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DestroyOnCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DeactivateOnCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DeactivateOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCompleted_ReactivateAfterDurationOnCanceled:
		if (Target->GetInteractionFinishMethod() == EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DeactivateOnCompleted ||
			Target->GetInteractionFinishMethod() == EInteractionFinishMethod::DeactivateOnCompleted_ReactivateAfterDurationOnCanceled)
		{
			// Completed 时停用
			Add_To_Deactivated_Targets(Target);
		}
		else
		{
			// Completed 时重新激活
			Target->On_Added_To_Pending_Target();
		}
		break;

		// 停用
	case EInteractionFinishMethod::DeactivateOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCompletedOrCanceled:
		Add_To_Deactivated_Targets(Target);
		break;

		// 不操作（Completed 时无动作）
	case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled:
	case EInteractionFinishMethod::DeactivateOnCanceled:
	default:
		break;
	}
}

// 辅助函数处理 Canceled
void UManager_Interactor::HandleCanceled(UManager_InteractionTarget* Target)
{
	switch (Target->GetInteractionFinishMethod())
	{
		// 销毁
	case EInteractionFinishMethod::DestroyOnCanceled:
	case EInteractionFinishMethod::DestroyOnCompletedOrCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DestroyOnCanceled:
	case EInteractionFinishMethod::DeactivateOnCompleted_DestroyOnCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DeactivateOnCanceled:
		if (Target->GetOwnerReference())
		{
			Target->GetOwnerReference()->K2_DestroyActor();
		}
		break;

		// 重新激活
	case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompletedOrCanceled:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DestroyOnCompleted:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DeactivateOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCompleted_ReactivateAfterDurationOnCanceled:
	case EInteractionFinishMethod::DeactivateOnCanceled_ReactivateAfterDurationOnCompleted:
		if (Target->GetInteractionFinishMethod() == EInteractionFinishMethod::DeactivateOnCanceled_ReactivateAfterDurationOnCompleted ||
			Target->GetInteractionFinishMethod() == EInteractionFinishMethod::DeactivateOnCompleted_ReactivateAfterDurationOnCanceled)
		{
			// Canceled 时停用
			Add_To_Deactivated_Targets(Target);
		}
		else
		{
			// Canceled 时重新激活
			Target->On_Added_To_Pending_Target();
		}
		break;

		// 停用
	case EInteractionFinishMethod::DeactivateOnCanceled:
	case EInteractionFinishMethod::DeactivateOnCompletedOrCanceled:
		Add_To_Deactivated_Targets(Target);
		break;

		// 不操作
	case EInteractionFinishMethod::DestroyOnCompleted:
	case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCompleted:
	case EInteractionFinishMethod::DeactivateOnCanceled_DestroyOnCompleted:
	default:
		break;
	}
}


void UManager_Interactor::Add_To_Pending_Targets(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 加入待处理列表

	if (!InteractionTarget)
	{
		return;
	}

	// 1. 将目标添加到 PendingTargets（去重）
	PendingTargets.AddUnique(InteractionTarget);

	// 2. 从 PointOfInterests 中移除（Add = false）
	Server_Update_PointOfInterests(false, InteractionTarget);

	// 3. 从 InteractionTargets 中移除（Add = false）
	Server_Update_InteractionTargets(false, InteractionTarget);

	// 4. 检查定时器句柄是否有效
	bool bTimerValid = UKismetSystemLibrary::K2_IsValidTimerHandle(PendingTarget_TimerHandle);

	if (!bTimerValid)
	{
		// 5. 如果定时器无效，创建新的循环定时器
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UManager_Interactor::Check_For_Pending_Targets);

		GetWorld()->GetTimerManager().SetTimer(
			PendingTarget_TimerHandle,
			TimerDelegate,
			PendingTargetCheckInterval,
			true,   // bLooping = true
			0.0f    // InitialStartDelay
		);
	}
	// 如果定时器已有效，则不做任何操作（保持现有定时器运行）

}

void UManager_Interactor::Add_To_Deactivated_Targets(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 加入已停用列表

	if (!InteractionTarget)
	{
		return;
	}

	// 1. 将目标添加到 DeactivatedTargets（去重）
	DeactivatedTargets.AddUnique(InteractionTarget);

	// 2. 从 PointOfInterests 中移除（Add = false）
	Server_Update_PointOfInterests(false, InteractionTarget);

	// 3. 从 InteractionTargets 中移除（Add = false）
	Server_Update_InteractionTargets(false, InteractionTarget);

	// 4. 调用目标自身的 On_Deactivated 函数
	InteractionTarget->On_Deactivated();
}

void UManager_Interactor::Remove_From_Deactivated_Targets(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 从已停用列表移除

	if (!InteractionTarget)
	{
		return;
	}

	DeactivatedTargets.Remove(InteractionTarget);
}

void UManager_Interactor::Check_For_Pending_Targets()
{
	// TODO: 定时检查待处理目标
	// 1. 如果 PendingTargets 为空，清除定时器并返回
	if (PendingTargets.IsEmpty())
	{
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(PendingTarget_TimerHandle);
			PendingTarget_TimerHandle.Invalidate();
		}
		return;
	}

	// 2. 遍历所有待处理目标
	for (UManager_InteractionTarget* LocCurrentPendingTarget : PendingTargets)
	{
		if (!LocCurrentPendingTarget)
		{
			continue;
		}

		// 3. 获取当前游戏时间
		double CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);

		// 4. 计算距离上次交互的时间差
		double TimeSinceLastInteract = CurrentTime - LocCurrentPendingTarget->GetLastInteractedTime();
		double AbsTimeDiff = UKismetMathLibrary::Abs(TimeSinceLastInteract);

		// 5. 检查是否已达到重新激活时间
		double ReactivationDuration = LocCurrentPendingTarget->GetReactivationDuration();
		if (AbsTimeDiff >= ReactivationDuration)
		{
			// 6. 重新激活目标
			On_InteractionTarget_Reactivated(LocCurrentPendingTarget);
		}
	}

	// 注意：如果循环结束后 PendingTargets 变为空，下次定时器触发时会在步骤1中清除
}

void UManager_Interactor::Set_Target_Highlighted(UManager_InteractionTarget* InteractionTarget, bool IsHighlighted)
{
	// TODO: 切换目标高亮（描边材质）

	if (!InteractionTarget)
	{
		return;
	}

	// 1. 更新动态材质的轮廓颜色参数
	if (Outline_DynamicMaterial)
	{
		Outline_DynamicMaterial->SetVectorParameterValue(
			FName(TEXT("Outline Color")),
			InteractionTarget->GetHighlightColor()
		);
	}

	// 2. 调用目标自身的高亮设置函数
	InteractionTarget->Set_Highlight(IsHighlighted);

}

// ============================================================================
// 事件回调
// ============================================================================
void UManager_Interactor::On_InteractionTarget_Destroyed(AActor* DestroyedActor)
{
	// TODO: 目标销毁后的清理逻辑

	if (!DestroyedActor)
	{
		return;
	}

	// 1. 从销毁的 Actor 中获取 InteractionTarget 组件
	UManager_InteractionTarget* LocTarget = Cast<UManager_InteractionTarget>(
		DestroyedActor->GetComponentByClass(UManager_InteractionTarget::StaticClass())
	);
	if (!LocTarget)
	{
		return;
	}

	// 2. 从 InteractionTargets 数组中移除
	InteractionTargets.Remove(LocTarget);

	// 3. 从 PointOfInterests 数组中移除
	PointOfInterests.Remove(LocTarget);

	// 4. 通知客户端（清理 UI 等）
	Client_On_Interaction_Target_Destroyed(LocTarget);

	// 5. 清除 DestroyedActor 的 OnDestroyed 委托绑定
	DestroyedActor->OnDestroyed.RemoveAll(this);

}

void UManager_Interactor::On_InteractionTarget_Reactivated(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 目标重新激活后的处理

	if (!InteractionTarget)
	{
		return;
	}

	// 1. 从 PendingTargets 中移除
	PendingTargets.Remove(InteractionTarget);

	// 2. 设置 InteractionEnabled = true
	InteractionTarget->SetInteractionEnabled(true);

	// 3. 检查 OwnerController 是否在 InteractionTarget 的 AssignedInteractors 中
	bool bIsAssigned = false;
	if (OwnerController)
	{
		bIsAssigned = InteractionTarget->GetAssignedInteractors().Contains(OwnerController);
	}

	// 4. 如果已分配，检查 Pawn 的重叠组件
	if (bIsAssigned)
	{
		APawn* Pawn = OwnerController ? OwnerController->GetPawn() : nullptr;
		if (Pawn)
		{
			TArray<UPrimitiveComponent*> LocPawnOverlappingComponents;
			Pawn->GetOverlappingComponents(LocPawnOverlappingComponents);

			// 检查 InnerZone - 使用隐式向上转换，不需要 Cast
			USphereComponent* InnerZone = InteractionTarget->GetInnerZone();
			bool bInnerZoneOverlapping = false;
			if (InnerZone)
			{
				// 直接赋值即可，USphereComponent* 可隐式转换为 UPrimitiveComponent*
				UPrimitiveComponent* InnerZonePrim = InnerZone;
				bInnerZoneOverlapping = LocPawnOverlappingComponents.Contains(InnerZonePrim);
			}

			if (bInnerZoneOverlapping)
			{
				Server_Update_PointOfInterests(true, InteractionTarget);
				Server_Update_InteractionTargets(true, InteractionTarget);
			}
			else
			{
				// 检查 OuterZone
				USphereComponent* OuterZone = InteractionTarget->GetOuterZone();
				bool bOuterZoneOverlapping = false;
				if (OuterZone)
				{
					UPrimitiveComponent* OuterZonePrim = OuterZone;  // 同样直接赋值
					bOuterZoneOverlapping = LocPawnOverlappingComponents.Contains(OuterZonePrim);
				}

				if (bOuterZoneOverlapping)
				{
					Server_Update_PointOfInterests(true, InteractionTarget);
				}
			}
		}
	}

	// 5. 触发 OnInteractionReactivated 委托
	APawn* PawnForDelegate = OwnerController ? OwnerController->GetPawn() : nullptr;
	InteractionTarget->NotifyInteractionReactivated(PawnForDelegate);

}

void UManager_Interactor::On_PointOfInterest_Updated_ServerSide(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 服务器侧兴趣点更新


	if (!InteractionTarget)
	{
		return;
	}

	if (Add)
	{
		// === 添加模式 ===
		// 1. 检查目标是否可交互
		if (IsInteractable(InteractionTarget))
		{
			// 2. 添加到 PointOfInterests（去重）
			PointOfInterests.AddUnique(InteractionTarget);

			// 3. 绑定 OnDestroyed 事件（如果尚未绑定）
			AActor* OwnerActor = InteractionTarget->GetOwnerReference();
			if (OwnerActor && !OwnerActor->OnDestroyed.IsAlreadyBound(this, &UManager_Interactor::On_InteractionTarget_Destroyed))
			{
				OwnerActor->OnDestroyed.AddDynamic(this, &UManager_Interactor::On_InteractionTarget_Destroyed);
			}

			// 4. 通知客户端更新
			Client_Update_PointOfInterests(true, InteractionTarget);
		}
		else
		{
			// 5. 如果不可交互，检查是否支持重新激活
			if (InteractionTarget->IsReactivationEnabled())
			{
				// 6. 添加到待处理列表
				Add_To_Pending_Targets(InteractionTarget);
			}
			// 否则不做任何操作
		}
	}
	else
	{
		// === 移除模式 ===
		// 7. 从 PointOfInterests 中移除
		PointOfInterests.Remove(InteractionTarget);

		// 8. 通知客户端更新
		Client_Update_PointOfInterests(false, InteractionTarget);
	}

}

void UManager_Interactor::On_PointOfInterest_Updated_ClientSide(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 客户端侧兴趣点更新


	if (!InteractionTarget)
	{
		return;
	}

	if (Add)
	{
		// === 添加模式 ===
		// 1. 更新目标 Widget 信息（屏幕边距和半径百分比）
		InteractionTarget->Update_Widget_Info(WidgetScreenMargin, ScreenRadiusPercent);

		// 2. 尝试查找空闲 Widget
		UInteractionWidget* EmptyWidget = Find_Empty_Widget();

		// 3. 如果找到空闲 Widget，直接使用它
		if (EmptyWidget)
		{
			// 调用 Widget 的 Update_InteractionTarget

			EmptyWidget->UpdateInteractionTargets(InteractionTarget);

			//UFunction* UpdateFunc = EmptyWidget->FindFunction(FName("Update_InteractionTarget"));
			//if (UpdateFunc)
			//{
			//	struct
			//	{
			//		UManager_InteractionTarget* InteractionTarget;
			//	} Params;
			//	Params.InteractionTarget = InteractionTarget;
			//	EmptyWidget->ProcessEvent(UpdateFunc, &Params);
			//}
		}
		else
		{
			// 4. 没有空闲 Widget，检查是否为本地玩家控制器
			if (!OwnerController || !OwnerController->IsLocalPlayerController())
			{
				return;
			}

			// 5. 创建新的 Widget
			UClass* WidgetClass = LoadClass<UInteractionWidget>(nullptr, TEXT("/Game/InteractionManager/UserInterface/WB_InteractionTarget.WB_InteractionTarget_C"));
			if (!WidgetClass)
			{
				return;
			}

			UInteractionWidget* NewWidget = CreateWidget<UInteractionWidget>(OwnerController, WidgetClass);
			if (!NewWidget)
			{
				return;
			}

			// 6. 添加到 WidgetPool（去重）
			WidgetPool.AddUnique(NewWidget);

			// 7. 添加到玩家屏幕
			NewWidget->AddToPlayerScreen(0);

			// 8. 调用 Widget 的 Update_InteractionTarget
			NewWidget->UpdateInteractionTargets(InteractionTarget);
			//UFunction* UpdateFunc = NewWidget->FindFunction(FName("Update_InteractionTarget"));
			//if (UpdateFunc)
			//{
			//	struct
			//	{
			//		UManager_InteractionTarget* InteractionTarget;
			//	} Params;
			//	Params.InteractionTarget = InteractionTarget;
			//	NewWidget->ProcessEvent(UpdateFunc, &Params);
			//}
		}
	}
	else
	{
		// === 移除模式 ===
		// 1. 查找对应的 Widget
		UInteractionWidget* ExistingWidget = Find_Widget_By_InteractionTarget(InteractionTarget);

		// 2. 如果找到，调用 Update_InteractionTarget（用于清除）
		if (ExistingWidget)
		{
			ExistingWidget->UpdateInteractionTargets(InteractionTarget);
			//UFunction* UpdateFunc = ExistingWidget->FindFunction(FName("UpdateInteractionTarget"));
			//if (UpdateFunc)
			//{
			//	struct
			//	{
			//		UManager_InteractionTarget* InteractionTarget;
			//	} Params;
			//	Params.InteractionTarget = InteractionTarget;
			//	ExistingWidget->ProcessEvent(UpdateFunc, &Params);
			//}
		}
	}

}

void UManager_Interactor::On_InteractionTarget_Updated_ServerSide(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 服务器侧交互目标更新

	if (!InteractionTarget)
	{
		return;
	}

	if (Add)
	{
		// === 添加模式 ===
		// 1. 检查目标是否可交互
		if (IsInteractable(InteractionTarget))
		{
			// 2. 通知客户端更新（添加到 InteractionTargets）
			Client_Update_InteractionTargets(true, InteractionTarget);

			// 3. 添加到 InteractionTargets（去重）
			InteractionTargets.AddUnique(InteractionTarget);
		}
		// 如果不可交互，不做任何操作
	}
	else
	{
		// === 移除模式 ===
		// 1. 从 InteractionTargets 中移除
		InteractionTargets.Remove(InteractionTarget);

		// 2. 通知客户端更新（从 InteractionTargets 移除）
		Client_Update_InteractionTargets(false, InteractionTarget);
	}

}

void UManager_Interactor::On_InteractionTarget_Updated_ClientSide(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 客户端侧交互目标更新

	if (!InteractionTarget)
	{
		return;
	}

	// 1. 查找对应的 Widget
	UInteractionWidget* TargetWidget = Find_Widget_By_InteractionTarget(InteractionTarget);

	// 2. 如果找到，调用 Update_ContentState
	if (TargetWidget)
	{
		TargetWidget->UpdateContentStates(Add);
		//UFunction* UpdateFunc = TargetWidget->FindFunction(FName("UpdateContentState"));
		//if (UpdateFunc)
		//{
		//	struct
		//	{
		//		bool IsInteraction;
		//	} Params;
		//	Params.IsInteraction = Add;
		//	TargetWidget->ProcessEvent(UpdateFunc, &Params);
		//}
	}
	// 如果未找到，不做任何操作
}

void UManager_Interactor::On_New_Target_Selected_ClientSide(UManager_InteractionTarget* NewTarget, bool IsSelected)
{
	// TODO: 客户端侧新目标选中处理

	// 1. 设置 BestInteractionTarget
	BestInteractionTarget = NewTarget;

	if (IsSelected)
	{
		// 2. 选中状态：设置高亮
		Set_Target_Highlighted(BestInteractionTarget, true);

		// 3. 查找对应的 Widget
		UInteractionWidget* MarkerWidget = Find_Widget_By_InteractionTarget(BestInteractionTarget);
		CurrentInteractionMarker = MarkerWidget;

		if (MarkerWidget)
		{
			// 4. 获取交互按键列表
			bool bSuccess = false;
			TArray<FKey> Keys;
			Get_InteractionKeys(bSuccess, Keys);

			// 5. 取第一个按键（如果存在）
			if (bSuccess && Keys.Num() > 0)
			{
				FKey FirstKey = Keys[0];
				// 调用 Widget 的 Set_Interaction_Key_Text 函数
				MarkerWidget->SetInteractionKeyTexts(FirstKey);
				//UFunction* SetKeyFunc = MarkerWidget->FindFunction(FName("SetInteractionKeyText"));
				//if (SetKeyFunc)
				//{
				//	struct
				//	{
				//		FKey Key;
				//	} Params;
				//	Params.Key = FirstKey;
				//	MarkerWidget->ProcessEvent(SetKeyFunc, &Params);
				//}
			}

			// 6. 调用 Widget 的 On_Widget_New_Target，IsNewTarget = true
			MarkerWidget->OnWidgetNewTargets(true);
			//UFunction* OnNewTargetFunc = MarkerWidget->FindFunction(FName("OnWidgetNewTarget"));
			//if (OnNewTargetFunc)
			//{
			//	struct
			//	{
			//		bool IsNewTarget;
			//	} Params;
			//	Params.IsNewTarget = true;
			//	MarkerWidget->ProcessEvent(OnNewTargetFunc, &Params);
			//}
		}
	}
	else
	{
		// 7. 取消选中状态
		if (CurrentInteractionMarker)
		{
			// 调用 Widget 的 On_Widget_New_Target，IsNewTarget = false
			CurrentInteractionMarker->OnWidgetNewTargets(false);
			//UFunction* OnNewTargetFunc = CurrentInteractionMarker->FindFunction(FName("OnWidgetNewTarget"));
			//if (OnNewTargetFunc)
			//{
			//	struct
			//	{
			//		bool IsNewTarget;
			//	} Params;
			//	Params.IsNewTarget = false;
			//	CurrentInteractionMarker->ProcessEvent(OnNewTargetFunc, &Params);
			//}
		}

		// 8. 清除 CurrentInteractionMarker
		CurrentInteractionMarker = nullptr;

		// 9. 取消高亮
		Set_Target_Highlighted(BestInteractionTarget, false);
	}

}

void UManager_Interactor::On_Interaction_Updated_Implementation(UManager_InteractionTarget* InteractionTarget, float Alpha, int32 InRepeated)
{
	// 安全检查
	if (!IsValid(InteractionTarget))
	{
		return;
	}

	// 获取 InteractorPawn（）
	APawn* InteractorPawn = IsValid(OwnerController) ? OwnerController->GetPawn() : nullptr;

	// 1. 本地广播委托（）
	InteractionTarget->OnInteractionUpdated.Broadcast(Alpha, Repeated, InteractorPawn);

	// 2. 如果不是单机，则转发到服务器（）
	if (!UKismetSystemLibrary::IsStandalone(this))
	{
		Server_On_Interaction_Updated(InteractionTarget, Alpha, Repeated, InteractorPawn);
	}
}

// ============================================================================
// 网络 RPC —— Server（_Implementation 由 UE 自动生成，这里实现服务器逻辑）
// ============================================================================
void UManager_Interactor::Server_On_Interaction_Updated_Implementation(UManager_InteractionTarget* InteractionTarget, float Alpha, int32 InRepeated, APawn* InteractorPawn)
{
	// TODO: 服务器侧交互进度更新
	if (InteractionTarget)
	{
		InteractionTarget->OnInteractionUpdated.Broadcast(Alpha, InRepeated, InteractorPawn);
	}
}

void UManager_Interactor::Server_On_Interaction_Begin_Implementation(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 服务器侧交互开始
	// 安全检查
	if (!IsValid(InteractionTarget))
	{
		return;
	}

	// 从 OwnerController 获取 Pawn
	APawn* InteractorPawn = IsValid(OwnerController) ? OwnerController->GetPawn() : nullptr;

	// 广播委托
	// 注意：即使在服务器端，广播也是本地执行，客户端绑定会各自触发
	InteractionTarget->OnInteractionBegin.Broadcast(InteractorPawn);

	// 更新自身状态
	IsInteracting = true;
}

void UManager_Interactor::Server_Request_Assign_Interactor_Implementation(UManager_InteractionTarget* InteractionTarget, bool Add)
{
	// TODO: 服务器侧分配交互器
	if (InteractionTarget)
	{
		InteractionTarget->Assign_Interactor(Add, OwnerController);
	}
}

void UManager_Interactor::Server_Update_PointOfInterests_Implementation(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 服务器侧更新兴趣点

	On_PointOfInterest_Updated_ServerSide(Add,InteractionTarget);


}

void UManager_Interactor::Server_Update_InteractionTargets_Implementation(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 服务器侧更新交互目标

	On_InteractionTarget_Updated_ServerSide(Add, InteractionTarget);
}

void UManager_Interactor::Server_On_Interaction_Finished_Implementation(UManager_InteractionTarget* InteractionTationrget, EInteractionResult Result)
{
	// TODO: 服务器侧交互结束

	Apply_Finish_Method(InteractionTationrget, Result);
}

// ============================================================================
// 网络 RPC —— Client（_Implementation 由 UE 自动生成，这里实现客户端逻辑）
// ============================================================================
void UManager_Interactor::Client_Check_Pressed_Key_Implementation()
{
	// TODO: 客户端侧检查按下的键
	Try_Take_Interaction();
}

void UManager_Interactor::Client_On_Interaction_Target_Destroyed_Implementation(UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 客户端侧目标销毁处理

	if (CurrentInteractionMarker) { CurrentInteractionMarker->UpdateInteractionTargets(nullptr); }

}

void UManager_Interactor::Client_Update_InteractionTargets_Implementation(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 客户端侧更新交互目标
	On_InteractionTarget_Updated_ClientSide(Add, InteractionTarget);
}

void UManager_Interactor::Client_Update_PointOfInterests_Implementation(bool Add, UManager_InteractionTarget* InteractionTarget)
{
	// TODO: 客户端侧更新兴趣点
	On_PointOfInterest_Updated_ClientSide(Add, InteractionTarget);
}

void UManager_Interactor::Client_Reset_Data_Implementation()
{
	// TODO: 客户端侧重置数据

	KeyJustPressed = false;
	LastPressedKey = FKey(NAME_None);
	CurrentHoldTime = 0.0;
	Repeated = 0;


}

void UManager_Interactor::Client_Set_New_Target_Implementation(UManager_InteractionTarget* NewTarget, bool IsSelected)
{
	// TODO: 客户端侧设置新目标
	On_New_Target_Selected_ClientSide(NewTarget, IsSelected);
}

void UManager_Interactor::Client_Update_Interaction_Text_Begin_Implementation(UManager_InteractionTarget* InteractionTarget)
{
}

void UManager_Interactor::Client_Reset_Progress_Implementation()
{
}

void UManager_Interactor::Client_Set_Interaction_Percent_Implementation(double Percent)
{
}

void UManager_Interactor::Client_Play_Completed_Animation_Implementation(UManager_InteractionTarget* Target, uint8 Result)
{
}



