// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 * 前端 UI 专用玩家控制器
 * 在 OnPossess 中自动设置 UI 摄像机视角，并执行硬件性能基准测试
 * 以获取可扩展性质量设置的推荐值。
 */
UCLASS()
class ADVANCEFRONTUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
	protected:

	/**
	 * 当控制器拥有 Pawn 时调用
	 * 自动切换到主菜单 UI 摄像机视角，并运行硬件基准测试
	 */
	virtual void OnPossess(APawn* InPawn) override;
};
