// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_PrimaryLayout.h"

UCommonActivatableWidgetContainerBase *UWidget_PrimaryLayout::FindRegisteredWidgetStackByTag(const FGameplayTag &WidgetStackTag) const
{
    if (!RegisteredWidgetStackMap.Contains(WidgetStackTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("UWidget_PrimaryLayout::FindRegisteredWidgetStackByTag() WidgetStackTag: %s not found in RegisteredWidgetStackMap"), *WidgetStackTag.ToString());
    }
    else
    {
        return RegisteredWidgetStackMap[WidgetStackTag];    
        
    }


    return nullptr;
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta=(Categories = "Frontend.WidgetStack"))FGameplayTag WidgetStackTag, UCommonActivatableWidgetContainerBase *WidgetStack)
{
    if (!IsDesignTime())
    {
       if (!RegisteredWidgetStackMap.Contains(WidgetStackTag))
       {
        RegisteredWidgetStackMap.Add(WidgetStackTag, WidgetStack);
       }
       else
       {
        UE_LOG(LogTemp, Warning, TEXT("UWidget_PrimaryLayout::RegisterWidgetStack() WidgetStackTag: %s already exists in RegisteredWidgetStackMap"), *WidgetStackTag.ToString());
       }
       {
        /* code */
       }
       
    }
    
}
