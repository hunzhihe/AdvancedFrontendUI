// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

/**
 * 
 */
 // 前置声明：通用可激活控件容器基类
class UCommonActivatableWidgetContainerBase;

/**
 * 主布局控件基类（抽象，不可直接实例化）
 * 禁用原生 Tick，使用 UE 的普通蓝图类型
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_PrimaryLayout : public UCommonUserWidget
{
    GENERATED_BODY()

public:
    /**
     * 根据指定的游戏标签查找已注册的控件堆栈容器
     * @param WidgetStackTag 控件堆栈的标签（通常用于标识不同区域的堆栈）
     * @return 找到的控件容器指针，未找到则返回 nullptr
     */
    UCommonActivatableWidgetContainerBase* FindRegisteredWidgetStackByTag(const FGameplayTag& WidgetStackTag) const;

protected:
    /**
     * 注册一个控件堆栈容器到标签映射表中
     * 仅供子类或蓝图调用（BlueprintCallable）
     * @param WidgetStackTag 控件堆栈的标签，受 "Frontend.WidgetStack" 类别限制（蓝图选择时只显示该类别的标签）
     * @param WidgetStack    要注册的控件堆栈容器指针
     */
    UFUNCTION(BlueprintCallable)
    void RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetStackTag,
        UCommonActivatableWidgetContainerBase* WidgetStack);

private:
    /** 标签到控件堆栈容器的映射表，用于快速查找。Transient 表示不参与序列化（运行时临时数据） */
    UPROPERTY(Transient)
    TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;
};
