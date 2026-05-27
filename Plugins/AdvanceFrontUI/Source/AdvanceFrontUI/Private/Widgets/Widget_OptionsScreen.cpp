// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "FrontendUIDebugHelper.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!ResetToDefaultAction.IsNull())
    {
        ResetToDefaultActionHandle = RegisterUIActionBinding(
        FBindUIActionArgs(
            ResetToDefaultAction,
            true,
            FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered))
       );
    }

    //ICommonInputModule::GetSettings().SetDefaultBackAction(true);

    RegisterUIActionBinding(
        FBindUIActionArgs(
            ICommonInputModule::GetSettings().GetDefaultBackAction(),
            true,
            FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered))
       );
    
   
}
void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
    FrontendUIDebugHelper::Log("Reset to default action triggered.");
}
void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
    DeactivateWidget();
    FrontendUIDebugHelper::Log("Back action triggered on options screen.");
}