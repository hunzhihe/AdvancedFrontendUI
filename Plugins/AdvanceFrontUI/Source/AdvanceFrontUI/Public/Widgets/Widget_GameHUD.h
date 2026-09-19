// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_GameHUD.generated.h"

/**
 * 游戏内 HUD 控件
 * 放置在 Frontend.WidgetStack.GameHud 堆栈中，游戏运行时始终显示在视口底层。
 *
 * 输入模式为 All — UI 输入与游戏输入并行处理，准星/小地图等 UI 元素可交互
 * 同时 WASD/鼠标等游戏操作不受影响。
 * 当 GameMenu（暂停菜单）被推开在上层时，HUD 保持可见但输入被菜单截获。
 */
UCLASS(Abstract, BlueprintType)
class ADVANCEFRONTUI_API UWidget_GameHUD : public UWidget_ActivatableBase
{
	GENERATED_BODY()

protected:
	// Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// End UUserWidget Interface

	// Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	// End UCommonActivatableWidget Interface

public:
	/** 获取当前缓存的玩家 Pawn（弱引用，可能为空） */
	UFUNCTION(BlueprintPure, Category = "GameHUD")
	APawn* GetCachedPlayerPawn() const { return CachedPlayerPawn.Get(); }

	/** 获取当前缓存的玩家控制器（弱引用，可能为空） */
	UFUNCTION(BlueprintPure, Category = "GameHUD")
	APlayerController* GetCachedPlayerController() const { return CachedPlayerController.Get(); }

protected:
	/**
	 * 每 Tick 调用的蓝图事件
	 * 在蓝图中覆写此事件来刷新所有 HUD 子控件（血条、弹药、小地图等）
	 * @param DeltaTime 上一帧到当前帧的时间间隔
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GameHUD")
	void OnHUDDataUpdated(float DeltaTime);

	/**
	 * 玩家 Pawn 变更时调用的蓝图事件
	 * 典型触发场景：玩家角色死亡后重生、切换载具等
	 * @param NewPawn 新的玩家 Pawn 引用（可能为空）
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GameHUD")
	void OnPlayerPawnChanged(APawn* NewPawn);

private:
	/** 在 Tick 中检测 Pawn 是否变更，若变更则触发 OnPlayerPawnChanged */
	void RefreshPlayerReferences();

	/** 缓存的玩家控制器弱引用（Tick 中自动维护） */
	TWeakObjectPtr<APlayerController> CachedPlayerController;

	/** 缓存的玩家 Pawn 弱引用（Tick 中自动维护，检测变更以触发 OnPlayerPawnChanged） */
	TWeakObjectPtr<APawn> CachedPlayerPawn;
};
