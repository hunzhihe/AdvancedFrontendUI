// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendUIFunctionLibrary.h"
#include "FrontendUIDebugHelper.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"

TSoftClassPtr<UWidget_ActivatableBase> UFrontendUIFunctionLibrary::GetFrontendWidgetFromTag(
    UPARAM(meta = (Categories = "Frontend.Widget")) FGameplayTag WidgetTag)
{
	// 从开发者设置中查找标签对应的控件类
    const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

    if(FrontendDeveloperSettings->FrontendWidgetMap.Contains(WidgetTag))
    {
        return FrontendDeveloperSettings->FrontendWidgetMap.FindRef(WidgetTag);
    }
    else
    {
    	// 未找到时输出调试日志
        FrontendUIDebugHelper::Log(FString::Printf(TEXT("Widget Tag %s not found in FrontendDeveloperSettings"), *WidgetTag.ToString()));
    }
    return TSoftClassPtr<UWidget_ActivatableBase>();
}

TSoftObjectPtr<UTexture2D> UFrontendUIFunctionLibrary::GetOptionsSoftImage(
    UPARAM(meta = (Categories = "Frontend.Image"))FGameplayTag ImageTag)
{
	// 从开发者设置中查找标签对应的图片资产（断言确保存在）
    const UFrontendDeveloperSettings* FrontendDeveloperSettings = GetDefault<UFrontendDeveloperSettings>();

    checkf(FrontendDeveloperSettings->OptionsScreenSoftImageMap.Contains(ImageTag),
        TEXT("Could not find an Image accociated with tag %s"), *ImageTag.ToString());

    return FrontendDeveloperSettings->OptionsScreenSoftImageMap.FindRef(ImageTag);
}
