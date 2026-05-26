// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncAction/AsyncAction_PushConfirmScreen.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendGameplayTags.h"


UAsyncAction_PushConfirmScreen *UAsyncAction_PushConfirmScreen::PushConfirmScreen(UObject *WorldContextObject, FText ScreenTitle, FText ScreenMessage, EConfirmScreenType ConfirmScreenType)
{

    if (UWorld *World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        if (UFrontendUISubsystem *FrontendUISubsystem = UFrontendUISubsystem::Get(World))
        {
            UAsyncAction_PushConfirmScreen *Node = NewObject<UAsyncAction_PushConfirmScreen>();
            Node->CachedOwningWorld = World;
            Node->CachedConfirmScreenType = ConfirmScreenType;
            Node->CachedScreenTitle = ScreenTitle;
            Node->CachedScreenMessage = ScreenMessage;

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
        FrontendUISubsystem->PushConfirmScreenToModelStackAsync(
            CachedConfirmScreenType,
            CachedScreenTitle,
            CachedScreenMessage,
            [this](EConfirmScreenButtonType ClickedButtonType) {
                
                OnButtonClicked.Broadcast(ClickedButtonType);

                SetReadyToDestroy(); // 完成后自动结束节点生命周期，避免内存泄漏
            }
        );
  }
}