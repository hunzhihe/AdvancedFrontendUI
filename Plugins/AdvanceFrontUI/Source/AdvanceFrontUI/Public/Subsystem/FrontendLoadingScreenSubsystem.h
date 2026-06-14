// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

/**
 * 前端加载画面子系统（GameInstanceSubsystem + FTickableGameObject）
 * 监听地图加载事件（PreLoadMap/PostLoadMap），在关卡切换期间显示加载画面。
 * 通过 Tick 循环检查是否需要显示/移除加载画面，支持编辑器下的行为配置。
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem,public FTickableGameObject
{
	GENERATED_BODY()

public:

	/** 加载原因更新委托，广播当前的加载进度描述 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingReason);

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;

	/* Begin USystem Interface */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection);

	virtual void Deinitialize();
	/* End USystem Interface */

	/* Begin FTickableGameObject Interface */
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	/* End FTickableGameObject Interface */


private:
	/** 地图预加载回调 */
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);

	/** 地图加载完成回调 */
	void OnMapPostLoaded(UWorld* LoadedWorld);

	/** 每帧检查并更新加载画面状态 */
	void TryUpdataLoadingScreen();

	/** 检查是否有有效的预加载画面正在运行 */
	bool IsPreLoadScreenActive() const;

	/** 判断是否应该显示加载画面 */
	bool ShouldShowLoadingScreen();

	/** 检查世界状态是否需要显示加载画面 */
	bool CheckTheNeedToShowLoadingScreen();

	/** 如果尚未显示则创建并显示加载画面 */
	void TryDispalyLoadingScreenIfNone();

	/** 移除当前显示的加载画面 */
	void TryRemovedLoadingScreen();

	/** 通知所有实现了 IFrontendUILoadScreenInterface 的对象加载画面可见性变化 */
	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);

	/** 是否正在加载地图 */
	bool bIsCurrentlyLoadingMap = false;

	/** 加载画面保持显示的起始时间（-1 表示未开始计时） */
	float HoldLoadingScreenStartUpTime = -1.f;

	/** 当前的加载原因描述 */
	FString CurrentLoadingReason;

	/** 缓存的已创建加载画面 Slate 控件 */
	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;
};
