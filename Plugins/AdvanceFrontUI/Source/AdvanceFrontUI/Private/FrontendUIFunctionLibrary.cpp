// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendUIFunctionLibrary.h"
#include "FrontendUIDebugHelper.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendUIFunctionLibrary::GetFrontendWidgetFromTag(
    UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag WidgetTag)
{
    const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();
   
    if(FrontendDeveloperSettings->FrontendWidgetMap.Contains(WidgetTag))
    {
        return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(WidgetTag);
    }
    else
    {
        FrontendUIDebugHelper::Log(FString::Printf(TEXT("Widget Tag %s not found in FrontendDeveloperSettings"), *WidgetTag.ToString()));
    }
    return TSoftClassPtr<UWidget_ActivatableBase>();
}
