// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AsyncActionSoftWidget.generated.h"

class APlayerController;
class UWidget_ActivatableBase;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, UWidget_ActivatableBase*, PushedWidget);
/**
 * 
 */
 /**
  * 异步操作：将软引用控件推送到堆栈
  * 继承自 UBlueprintAsyncActionBase，可在蓝图异步节点中使用
  *
  * 功能：异步加载并推送一个软引用的可激活控件到指定的控件堆栈中，
  *       支持在推送完成后执行蓝图异步节点的后续逻辑
  */
UCLASS()
class ADVANCEFRONTUI_API UAsyncActionSoftWidget : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:

	//begin UBlueprintAsyncActionBase interface
    virtual void Activate() override;
	//end UBlueprintAsyncActionBase interface

    /**
     * 创建一个异步操作节点，用于将软引用控件推送到堆栈
     *
     * @param WorldContextObject          世界上下文对象，用于获取子系统等信息
     * @param OwningPlayerController      拥有该控件的玩家控制器（通常用于输入和焦点管理）
     * @param SoftWidgetClass             软引用的控件类（必须继承自 UWidget_ActivatableBase）
     * @param WidgetStackTag              目标控件堆栈的标签（受 "Frontend.WidgetStack" 类别限制）
     * @param bFocusOnNewlyPushedWidget   是否将焦点设置到新推送的控件上（默认为 true）
     * @return 构造好的异步操作对象，可在蓝图中绑定完成事件（OnPushCompleted 等）
     *
     * 注意：此函数仅供蓝图内部使用（BlueprintInternalUseOnly），
     *       实际生成的是蓝图异步节点的执行引脚。
     */
    UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true"))
    static UAsyncActionSoftWidget* PushSoftWidgetToStack(
        const UObject* WorldContextObject,
        APlayerController* OwningPlayerController,
        TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass,
        UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetStackTag,
        bool bFocusOnNewlyPushedWidget = true
    );

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

    UPROPERTY(BlueprintAssignable)
    FOnPushSoftWidgetDelegate AfterPush;


private:
	TWeakObjectPtr<APlayerController> CachedOwningPlayerController;
    TWeakObjectPtr<UWorld> CachedOwningWorld;
    TSoftClassPtr<UWidget_ActivatableBase> CachedPushedWidget;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;

};
