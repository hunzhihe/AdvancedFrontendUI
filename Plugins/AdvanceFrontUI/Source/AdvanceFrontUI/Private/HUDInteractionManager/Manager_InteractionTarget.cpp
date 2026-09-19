// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDInteractionManager/Manager_InteractionTarget.h"
#include "HUDInteractionManager/Manager_Interactor.h"
#include "HUDInteractionManager/InteractionEnums.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h" 
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"


// ============================================================================
// 构造函数
// ============================================================================
UManager_InteractionTarget::UManager_InteractionTarget()
{
	// 该组件无需每帧 Tick（蓝图中无 ReceiveTick）
	PrimaryComponentTick.bCanEverTick = false;
}

UManager_Interactor* UManager_InteractionTarget::GetInteractorManager(AController* Controller)
{
    if (!Controller)
        return nullptr;
    // 方式1：直接获取组件
    return Cast<UManager_Interactor>(Controller->GetComponentByClass(UManager_Interactor::StaticClass()));
    // 方式2：通过 GameInstance 子系统（如果使用子系统）
    // return UGameplayStatics::GetGameInstance(this)->GetSubsystem<UManager_InteractorSubsystem>();
}

// ============================================================================
// 生命周期（对应蓝图 ReceiveBeginPlay）
// ============================================================================
void UManager_InteractionTarget::BeginPlay()
{
	Super::BeginPlay();

	// TODO: 实现初始化逻辑（蓝图 BeginPlay 中的连线）

     // 1. 构建 Owner 必要组件（需根据实际蓝图实现）
    Construct_Owner_Essentials();

    // 2. 构建重叠检测球体
    Construct_Overlap_Zones();

    // 3. 构建高亮组件列表
    Construct_HighlightedComponents();

    // 4. 选择标记目标组件
    Select_MarkerComponent(MarkerComponentName);

    // 5. 绑定委托事件
    OnInteractionBegin.AddDynamic(this, &UManager_InteractionTarget::On_Interaction_Begin_Event);
    OnInteractionEnd.AddDynamic(this, &UManager_InteractionTarget::On_Interaction_End_Event);
}

// ============================================================================
// 构造 / 初始化
// ============================================================================
void UManager_InteractionTarget::Construct_HighlightedComponents()
{
	// TODO: 将 ComponentsToHighlight 中的组件加入 HighlightedComponents

    // 1. 获取 Owner 并保存其 RootComponent 到局部变量 LocOwnerRoot
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return;
    }
    USceneComponent* LocOwnerRoot = OwnerActor->GetRootComponent();
    if (!LocOwnerRoot)
    {
        return;
    }

    // 2. 处理根组件本身（如果它是 PrimitiveComponent）
    //    注意：根组件可能不是 PrimitiveComponent，需要 DynamicCast 检测
    UPrimitiveComponent* RootAsPrimitive = Cast<UPrimitiveComponent>(LocOwnerRoot);
    if (RootAsPrimitive)
    {
        // 获取根组件名称，检查是否在 ComponentsToHighlight 中
        FString RootName = RootAsPrimitive->GetName();
        if (ComponentsToHighlight.Contains(RootName))
        {
            // 添加到 HighlightedComponents（去重）
            HighlightedComponents.AddUnique(RootAsPrimitive);
        }
    }

    // 3. 获取 LocOwnerRoot 的所有子组件（包括所有后代）
    TArray<USceneComponent*> ChildrenComponents;
    LocOwnerRoot->GetChildrenComponents(true, ChildrenComponents); // bIncludeAllDescendants = true

    // 4. 遍历每个子组件
    for (USceneComponent* ChildComp : ChildrenComponents)
    {
        if (!ChildComp)
        {
            continue;
        }

        // 尝试转换为 PrimitiveComponent
        UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ChildComp);
        if (!PrimComp)
        {
            continue; // 不是 PrimitiveComponent，跳过
        }

        // 获取组件名称，检查是否在 ComponentsToHighlight 中
        FString CompName = PrimComp->GetName();
        if (ComponentsToHighlight.Contains(CompName))
        {
            // 添加到 HighlightedComponents（去重）
            HighlightedComponents.AddUnique(PrimComp);
        }
    }

}

void UManager_InteractionTarget::Construct_Overlap_Zones()
{
	// TODO: 创建并配置 InnerZone / OuterZone 球形触发区，绑定重叠事件

    // 1. 获取Owner并获取所有SceneComponent
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return;
    }

    TArray<USceneComponent*> SceneComponents;
    OwnerActor->GetComponents<USceneComponent>(SceneComponents);

    // 2. 遍历查找名称匹配的组件，存入局部变量 LocComponentToAttach
    USceneComponent* LocComponentToAttach = nullptr;
    for (USceneComponent* Comp : SceneComponents)
    {
        if (Comp)
        {
            FString CompName = Comp->GetName();
            if (CompName.Equals(InteractionZone_ComponentToAttach, ESearchCase::IgnoreCase))
            {
                LocComponentToAttach = Comp;
                break;
            }
        }
    }

    // 3. 创建 InnerZone 和 OuterZone SphereComponent
    if (!InnerZone)
    {
        InnerZone = NewObject<USphereComponent>(OwnerActor, USphereComponent::StaticClass(), TEXT("InnerZone"));
        if (InnerZone)
        {
            InnerZone->RegisterComponent();
            OwnerActor->AddInstanceComponent(InnerZone);
            InnerZone->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        }
    }

    if (!OuterZone)
    {
        OuterZone = NewObject<USphereComponent>(OwnerActor, USphereComponent::StaticClass(), TEXT("OuterZone"));
        if (OuterZone)
        {
            OuterZone->RegisterComponent();
            OwnerActor->AddInstanceComponent(OuterZone);
            OuterZone->AttachToComponent(OwnerActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        }
    }

    // 4. 根据 EnableDebug 控制隐藏/显示
    bool bShouldHide = !EnableDebug; // 如果EnableDebug为真，则不隐藏；否则隐藏
    if (InnerZone)
    {
        InnerZone->SetHiddenInGame(bShouldHide, false);
    }
    if (OuterZone)
    {
        OuterZone->SetHiddenInGame(bShouldHide, false);
    }

    // 5. 如果找到了目标组件，将 InnerZone 和 OuterZone 附加到它上面
    if (LocComponentToAttach)
    {
        if (InnerZone)
        {
            InnerZone->AttachToComponent(LocComponentToAttach, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
        }
        if (OuterZone)
        {
            OuterZone->AttachToComponent(LocComponentToAttach, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
        }
    }

    // 6. 绑定重叠事件（先 Inner 后 Outer）
    if (InnerZone)
    {
        InnerZone->OnComponentBeginOverlap.AddDynamic(this, &UManager_InteractionTarget::On_InnerZone_BeginOverlap);
        InnerZone->OnComponentEndOverlap.AddDynamic(this, &UManager_InteractionTarget::On_InnerZone_EndOverlap);
    }
    if (OuterZone)
    {
        OuterZone->OnComponentBeginOverlap.AddDynamic(this, &UManager_InteractionTarget::On_OuterZone_BeginOverlap);
        OuterZone->OnComponentEndOverlap.AddDynamic(this, &UManager_InteractionTarget::On_OuterZone_EndOverlap);
    }

    // 7. 设置半径
    if (InnerZone)
    {
        InnerZone->SetSphereRadius(InnerZoneRadius, true);
    }
    if (OuterZone)
    {
        float OuterRadius = InnerZoneRadius + FMath::Max(OuterZoneExtent, 10.0f);
        OuterZone->SetSphereRadius(OuterRadius, true);
    }

}

void UManager_InteractionTarget::Construct_Owner_Essentials()
{
	// TODO: 缓存 OwnerReference、MarkerTargetComponent 等
    OwnerReference = GetOwner();

}

void UManager_InteractionTarget::Update_Widget_Info(FMargin InWidgetMargin, float InScreenRadiusPercent)
{
	// TODO: 更新控件边距与屏幕半径百分比
    WidgetMargin = InWidgetMargin;
    ScreenRadiusPercent = InScreenRadiusPercent;
}

// ============================================================================
// 交互状态 / 配置查询
// ============================================================================
bool UManager_InteractionTarget::IsInteractionEnabled()
{
	// TODO: 返回交互是否启用
	return InteractionEnabled;
}

void UManager_InteractionTarget::On_Deactivated()
{
	// TODO: 目标停用时的清理逻辑
}

void UManager_InteractionTarget::Assign_Interactor(bool Add, AController* Interactor)
{
	// TODO: 添加/移除交互器控制器
    if (!Interactor)
    {
        return;
    }

    if (Add)
    {
        // 添加（去重）
        AssignedInteractors.AddUnique(Interactor);
    }
    else
    {
        // 移除
        AssignedInteractors.Remove(Interactor);
    }
}

void UManager_InteractionTarget::On_Added_To_Pending_Target()
{
	// TODO: 被加入待处理列表时的回调
}

bool UManager_InteractionTarget::IsReactivationEnabled()
{
	// TODO: 返回是否启用重新激活
    switch (FinishMethod)
    {
    //case EInteractionFinishMethod::DestroyOnCompleted:
    //    break;
    //case EInteractionFinishMethod::DestroyOnCanceled:
    //    break;
    //case EInteractionFinishMethod::DestroyOnCompletedOrCanceled:
    //    break;
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompletedOrCanceled:
       
    case EInteractionFinishMethod::DeactivateOnCompleted:
        
    case EInteractionFinishMethod::DeactivateOnCanceled:
        
    case EInteractionFinishMethod::DeactivateOnCompletedOrCanceled:
       
    case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DestroyOnCompleted:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DeactivateOnCompleted:
        
    case EInteractionFinishMethod::DeactivateOnCanceled_ReactivateAfterDurationOnCompleted:
        
    case EInteractionFinishMethod::DeactivateOnCanceled_DestroyOnCompleted:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DestroyOnCanceled:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DeactivateOnCanceled:
        
    case EInteractionFinishMethod::DeactivateOnCompleted_ReactivateAfterDurationOnCanceled:
        
    case EInteractionFinishMethod::DeactivateOnCompleted_DestroyOnCanceled:
        
        return true;
    default:
        return false;
    }

	
}

bool UManager_InteractionTarget::Cancel_On_Release()
{
	// TODO: 返回松开按键是否取消交互

    switch (FinishMethod)
    {
    //case EInteractionFinishMethod::DestroyOnCompleted:
    //    break;
    case EInteractionFinishMethod::DestroyOnCanceled:
       
    case EInteractionFinishMethod::DestroyOnCompletedOrCanceled:
        
    //case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted:
    //    break;
    case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompletedOrCanceled:
        
    //case EInteractionFinishMethod::DeactivateOnCompleted:
    //    break;
    case EInteractionFinishMethod::DeactivateOnCanceled:
        
    case EInteractionFinishMethod::DeactivateOnCompletedOrCanceled:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DestroyOnCompleted:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCanceled_DeactivateOnCompleted:
        
    case EInteractionFinishMethod::DeactivateOnCanceled_ReactivateAfterDurationOnCompleted:
        
    case EInteractionFinishMethod::DeactivateOnCanceled_DestroyOnCompleted:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DestroyOnCanceled:
        
    case EInteractionFinishMethod::ReactivateAfterDurationOnCompleted_DeactivateOnCanceled:
        
    case EInteractionFinishMethod::DeactivateOnCompleted_ReactivateAfterDurationOnCanceled:
        
    case EInteractionFinishMethod::DeactivateOnCompleted_DestroyOnCanceled:
        
        return true;
    default:
        return false;
    }
	
}

void UManager_InteractionTarget::Set_Highlight(bool IsHighlighted)
{
	// TODO: 切换高亮
    for (UPrimitiveComponent* Comp : HighlightedComponents)
    {
        if (IsValid(Comp))
        {
            Comp->SetRenderCustomDepth(IsHighlighted);
        }
    }
}

void UManager_InteractionTarget::Enable_Interaction(bool Enabled)
{
	// TODO: 启用/禁用交互
    // 1. 更新 InteractionEnabled 变量
    InteractionEnabled = Enabled;

    // 2. 遍历所有已分配的交互器
    for (AController* Controller : AssignedInteractors)
    {
        if (!IsValid(Controller))
        {
            continue;
        }

        // 获取该控制器对应的 InteractorManager
        UManager_Interactor* InteractorManager = GetInteractorManager(Controller);
        if (!InteractorManager)
        {
            continue;
        }

        if (InteractionEnabled)
        {
            // 启用交互：先移除停用列表，再触发重新激活事件
            InteractorManager->Remove_From_Deactivated_Targets(this);
            InteractorManager->On_InteractionTarget_Reactivated(this);
        }
        else
        {
            // 禁用交互：添加到停用列表
            InteractorManager->Add_To_Deactivated_Targets(this);
        }
    }
}

void UManager_InteractionTarget::Select_MarkerComponent(FString ComponentName)
{
	// TODO: 按名称选择标记组件
    // 1. 获取 Owner 及其 RootComponent
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return;
    }
    USceneComponent* LocOwnerRoot = OwnerActor->GetRootComponent();
    if (!LocOwnerRoot)
    {
        return;
    }

    // 2. 获取所有后代子组件（包括自身？蓝图只获取子组件，不包括根组件本身，因为根组件单独处理）
    //    蓝图使用 GetChildrenComponents(bIncludeAllDescendants=true) 只获取子级，不包括自身
    TArray<USceneComponent*> LocComponents;
    LocOwnerRoot->GetChildrenComponents(true, LocComponents); // bIncludeAllDescendants = true

    // 3. 遍历查找与 MarkerComponentName 匹配的组件
    USceneComponent* FoundComponent = nullptr;
    for (USceneComponent* Comp : LocComponents)
    {
        if (!Comp) continue;
        FString CompName = Comp->GetName();
        if (CompName.Equals(MarkerComponentName, ESearchCase::IgnoreCase))
        {
            FoundComponent = Comp;
            break; // 跳出循环（对应 Break 引脚）
        }
    }

    // 4. 如果找到，则赋值给 MarkerTargetComponent，否则赋值为根组件
    if (FoundComponent)
    {
        MarkerTargetComponent = FoundComponent;
    }
    else
    {
        MarkerTargetComponent = LocOwnerRoot;
    }
    //上面的逻辑已经保证 FoundComponent 不会为空（要么是子组件，要么是根组件），
    
}

// ============================================================================
// 重叠事件回调
// ============================================================================
void UManager_InteractionTarget::On_InnerZone_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: 进入内区（开始交互）

    // 1. 尝试将 OtherActor 转换为 Pawn
    APawn* OtherPawn = Cast<APawn>(OtherActor);
    if (!OtherPawn)
    {
        // 不是 Pawn，忽略
        return;
    }

    // 2. 获取该 Pawn 的控制器
    AController* Controller = OtherPawn->GetController();
    if (!IsValid(Controller))
    {
        // 控制器无效，忽略
        return;
    }

    // 3. 判断是否为本地玩家控制器
    if (!Controller->IsLocalPlayerController())
    {
        // 不是本地玩家，忽略
        return;
    }

    // 4. 获取 InteractorManager（蓝图宏“Get_InteractorManager”的 C++ 替代）
    //    以下代码为示例，您需要根据实际项目中的获取方式替换
    UManager_Interactor* InteractorManager = GetInteractorManager(Controller);
    if (!InteractorManager)
    {
        return;
    }

    // 5. 调用 Server 函数，Add=true，InteractionTarget=当前组件（self）
    InteractorManager->Server_Update_InteractionTargets(true, this);

}

void UManager_InteractionTarget::On_InnerZone_EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// TODO: 离开内区（结束交互）

    // 1. 尝试将 OtherActor 转换为 Pawn
    APawn* OtherPawn = Cast<APawn>(OtherActor);
    if (!OtherPawn)
    {
        return;
    }

    // 2. 获取该 Pawn 的控制器
    AController* Controller = OtherPawn->GetController();
    if (!IsValid(Controller))
    {
        return;
    }

    // 3. 判断是否为本地玩家控制器
    if (!Controller->IsLocalPlayerController())
    {
        return;
    }

    // 4. 获取 InteractorManager
    UManager_Interactor* InteractorManager = GetInteractorManager(Controller);
    if (!InteractorManager)
    {
        return;
    }

    // 5. 调用服务器 RPC，Add = false，表示移除
    InteractorManager->Server_Update_InteractionTargets(false, this);

}

void UManager_InteractionTarget::On_OuterZone_BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: 进入外区（显示 marker）
    // 1. 尝试将 OtherActor 转换为 Pawn
    APawn* OtherPawn = Cast<APawn>(OtherActor);
    if (!OtherPawn)
    {
        return;
    }

    // 2. 获取该 Pawn 的控制器
    AController* Controller = OtherPawn->GetController();
    if (!IsValid(Controller))
    {
        return;
    }

    // 3. 判断是否为本地玩家控制器
    if (!Controller->IsLocalPlayerController())
    {
        return;
    }

    // 4. 获取 InteractorManager（与 Inner 版本相同的方式）
    UManager_Interactor* InteractorManager = GetInteractorManager(Controller);
    if (!InteractorManager)
    {
        return;
    }

    // 5. 按顺序调用服务器 RPC
    // 先更新兴趣点（Point Of Interests）
    InteractorManager->Server_Update_PointOfInterests(true, this);
    // 再请求分配交互器（Assign Interactor）
    InteractorManager->Server_Request_Assign_Interactor(this, true);
}

void UManager_InteractionTarget::On_OuterZone_EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// TODO: 离开外区（隐藏 marker）
    // 1. 尝试将 OtherActor 转换为 Pawn
    APawn* OtherPawn = Cast<APawn>(OtherActor);
    if (!OtherPawn)
    {
        return;
    }

    // 2. 获取该 Pawn 的控制器
    AController* Controller = OtherPawn->GetController();
    if (!IsValid(Controller))
    {
        return;
    }

    // 3. 判断是否为本地玩家控制器
    if (!Controller->IsLocalPlayerController())
    {
        return;
    }

    // 4. 获取 InteractorManager
    UManager_Interactor* InteractorManager = GetInteractorManager(Controller);
    if (!InteractorManager)
    {
        return;
    }

    // 5. 按顺序调用服务器 RPC，Add = false，表示移除
    // 先更新兴趣点（Point Of Interests）
    InteractorManager->Server_Update_PointOfInterests(false, this);
    // 再请求分配交互器（Assign Interactor）
    InteractorManager->Server_Request_Assign_Interactor(this, false);
}


