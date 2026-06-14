// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FrontendUILoadScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType,MinimalAPI)
class UFrontendUILoadScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 前端 UI 加载画面接口
 * 实现此接口的对象（PlayerController、Pawn 等）可以接收加载画面的激活/停用通知。
 * UFrontendLoadingScreenSubsystem 在显示/隐藏加载画面时会遍历所有本地玩家通知这些事件。
 */
class ADVANCEFRONTUI_API IFrontendUILoadScreenInterface
{
	GENERATED_BODY()

public:

	/** 加载画面激活时调用（蓝图中实现） */
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();

	/** 加载画面停用时调用（蓝图中实现） */
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();


};
