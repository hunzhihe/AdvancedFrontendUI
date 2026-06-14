// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncAction_PushConfirmScreen.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendGameplayTags.h"


UAsyncAction_PushConfirmScreen *UAsyncAction_PushConfirmScreen::PushConfirmScreen(UObject *WorldContextObject, FText ScreenTitle, FText ScreenMessage, EConfirmScreenType ConfirmScreenType)
{
	// 从世界上下文获取 World 和 FrontendUISubsystem
    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        if (UFrontendUISubsystem *FrontendUISubsystem = UFrontendUISubsystem::Get(World))
        {
        	// 创建异步操作节点并缓存参数
            UAsyncAction_PushConfirmScreen *Node = NewObject<UAsyncAction_PushConfirmScreen>();
            Node->CachedOwningWorld = World;
            Node->CachedConfirmScreenType = ConfirmScreenType;
            Node->CachedScreenTitle = ScreenTitle;
            Node->CachedScreenMessage = ScreenMessage;

			// 注册到游戏实例以管理生命周期
            Node->RegisterWithGameInstance(World);

            return Node;
        }
    }

    return nullptr;
}
void UAsyncAction_PushConfirmScreen::Activate()
{
     UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
    if (FrontendUISubsystem)
    {
    	// 通过子系统异步推送确认画面到模态堆栈
        FrontendUISubsystem->PushConfirmScreenToModelStackAsync(
            CachedConfirmScreenType,
            CachedScreenTitle,
            CachedScreenMessage,
            [this](EConfirmScreenButtonType ClickedButtonType) {
                // 广播按钮点击事件给蓝图绑定的回调
                OnButtonClicked.Broadcast(ClickedButtonType);

                SetReadyToDestroy(); // 完成后自动结束节点生命周期，避免内存泄漏
            }
        );
  }
}