// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncActionSoftWidget.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendUIDebugHelper.h"



void UAsyncActionSoftWidget::Activate()
{
	   UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	   FrontendUISubsystem->PushSoftWidgetToStackAsync(
        CachedWidgetStackTag, CachedPushedWidget, [this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* CreatedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
                CreatedWidget->SetOwningPlayer(CachedOwningPlayerController.Get());
				OnWidgetCreatedBeforePush.Broadcast(CreatedWidget);
			}
			else if (PushState == EAsyncPushWidgetState::AfterPush)
			{
				AfterPush.Broadcast(CreatedWidget);

                if (bCachedFocusOnNewlyPushedWidget)
                {
                    if(CreatedWidget->IsFocusable())
                    {
                       CreatedWidget->SetFocus();
                    }
                      
                    //CreatedWidget->SetKeyboardFocus();
                }
				// 完成后自动结束节点生命周期，避免内存泄漏
				SetReadyToDestroy();
			}
		});
}

/**
 * 静态方法：创建一个异步操作节点，用于将软引用控件推送到堆栈
 * （注意：当前实现仅创建了节点并注册到游戏实例，实际推送逻辑可能需要后续补充或已在其他部分实现）
 *
 * @param WorldContextObject          世界上下文对象
 * @param OwningPlayerController      拥有控件的玩家控制器（当前未使用，预留）
 * @param SoftWidgetClass             软引用的控件类
 * @param WidgetStackTag              目标控件堆栈的标签
 * @param bFocusOnNewlyPushedWidget   是否自动聚焦新控件（当前未使用，预留）
 * @return 创建好的异步操作节点，如果失败则返回 nullptr
 */
UAsyncActionSoftWidget* UAsyncActionSoftWidget::PushSoftWidgetToStack(
    const UObject* WorldContextObject,
    APlayerController* OwningPlayerController,
    TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClass,
    UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetStackTag,
    bool bFocusOnNewlyPushedWidget)
{
    // 确保软引用控件类不为空
    if (!SoftWidgetClass.IsNull())
    {
        // 检查 GEngine 是否有效
        if (GEngine)
        {
            // 从上下文对象中获取世界（失败时仅记录日志并返回空）
            if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
            {
                // 创建异步操作节点对象
                UAsyncActionSoftWidget* Node = NewObject<UAsyncActionSoftWidget>();


				Node->CachedOwningWorld = World;
				Node->CachedOwningPlayerController = OwningPlayerController;
				Node->CachedPushedWidget = SoftWidgetClass;
				Node->CachedWidgetStackTag = WidgetStackTag;
				Node->bCachedFocusOnNewlyPushedWidget = bFocusOnNewlyPushedWidget;

                // 将节点注册到当前游戏实例，确保节点生命周期与游戏实例绑定
                Node->RegisterWithGameInstance(World);
                // 返回节点（实际推送逻辑可能在节点内部的其他回调中完成）
                return Node;
            }
        }
    }
    else
    {
        // 软引用类为空时输出错误日志（红色显示）
        FrontendUIDebugHelper::Log(FString::Printf(TEXT("PushSoftWidgetToStack was Passed a null Soft Widget Class for tag: %s"), *WidgetStackTag.ToString()), -1, FColor::Red);
    }
    // 创建失败返回空指针
    return nullptr;
}
