// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_GameHUD.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/GameViewportClient.h"
#include "CommonInputSubsystem.h"

void UWidget_GameHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 首次缓存玩家引用
	RefreshPlayerReferences();
}

void UWidget_GameHUD::NativeOnActivated()
{
	Super::NativeOnActivated();

	// 激活时刷新玩家引用（关卡切换后 Pawn 可能已变更）
	RefreshPlayerReferences();
}

void UWidget_GameHUD::NativeOnDeactivated()
{
	// 停用时清空缓存引用
	CachedPlayerController.Reset();
	CachedPlayerPawn.Reset();

	Super::NativeOnDeactivated();
}

TOptional<FUIInputConfig> UWidget_GameHUD::GetDesiredInputConfig() const
{
	// 返回 All 模式：UI 输入与游戏输入并行处理
	// 玩家可以同时操作 HUD 元素（如点击小地图）和游戏角色（WASD/鼠标瞄准）
	return FUIInputConfig(ECommonInputMode::All, EMouseCaptureMode::CaptureDuringMouseDown);
}

void UWidget_GameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 检测 Pawn 是否变更（死亡重生、切换角色等）
	RefreshPlayerReferences();

	// 通知蓝图刷新所有 HUD 子控件数据
	OnHUDDataUpdated(InDeltaTime);
}

void UWidget_GameHUD::RefreshPlayerReferences()
{
	APlayerController* PC = CachedPlayerController.Get();
	APawn* CurrentPawn = CachedPlayerPawn.Get();

	// 如果控制器引用失效，尝试重新获取
	if (!PC)
	{
		PC = GetOwningPlayer();
		if (PC)
		{
			CachedPlayerController = PC;
		}
	}

	// 检测 Pawn 是否变更
	if (PC)
	{
		APawn* NewPawn = PC->GetPawn();
		if (NewPawn != CurrentPawn)
		{
			CachedPlayerPawn = NewPawn;
			OnPlayerPawnChanged(NewPawn);
		}
	}
}
