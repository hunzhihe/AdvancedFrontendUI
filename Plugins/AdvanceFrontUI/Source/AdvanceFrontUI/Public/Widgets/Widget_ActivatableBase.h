// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Widget_ActivatableBase.generated.h"


class AFrontendPlayerController;
/**
 * 前端可激活控件的基类
 * 继承自 UCommonActivatableWidget，是所有前端 UI 控件的公共父类。
 * 提供对 AFrontendPlayerController 的缓存访问，避免重复查找。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_ActivatableBase : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
    /**
     * 获取拥有此控件的前端玩家控制器
     * 首次调用时缓存结果，后续调用直接返回缓存值
     * @return 前端玩家控制器指针，获取失败返回 nullptr
     */
    UFUNCTION(BlueprintPure, Category = "AdvanceFrontUI|ActivatableBase")
	AFrontendPlayerController* GetOwningFrontendPlayerController();

private:
    /** 缓存的拥有者玩家控制器，避免重复查找 */
    TWeakObjectPtr<AFrontendPlayerController> CacheOwningPlayerController;


};
