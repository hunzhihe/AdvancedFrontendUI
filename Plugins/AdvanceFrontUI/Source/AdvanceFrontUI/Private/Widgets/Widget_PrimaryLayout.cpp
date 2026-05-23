// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_PrimaryLayout.h"
#include "FrontendUIDebugHelper.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindRegisteredWidgetStackByTag(const FGameplayTag& WidgetStackTag) const
{
    // 检查指定的标签是否已在映射表中注册
    if (!RegisteredWidgetStackMap.Contains(WidgetStackTag))
    {
        // 未找到时输出警告日志，便于调试
        UE_LOG(LogTemp, Warning, TEXT("UWidget_PrimaryLayout::FindRegisteredWidgetStackByTag() WidgetStackTag: %s not found in RegisteredWidgetStackMap"), *WidgetStackTag.ToString());
    }
    else
    {
        // 找到则返回对应的控件堆栈容器指针
        return RegisteredWidgetStackMap[WidgetStackTag];
    }

    // 未找到返回空指针
    return nullptr;
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta = (Categories = "Frontend.WidgetStack")) FGameplayTag WidgetStackTag, UCommonActivatableWidgetContainerBase* WidgetStack)
{
    // 仅在运行时（非设计时）执行注册，避免编辑器中干扰
    if (!IsDesignTime())
    {
        // 防止重复注册同一个标签
        if (!RegisteredWidgetStackMap.Contains(WidgetStackTag))
        {
            // 将标签与控件堆栈容器添加到映射表中
            RegisteredWidgetStackMap.Add(WidgetStackTag, WidgetStack);

            // 可选：输出注册成功的调试日志（默认注释，按需启用）
            // FrontendUIDebugHelper::Log(FString::Printf(TEXT("UWidget_PrimaryLayout::RegisterWidgetStack() WidgetStackTag: %s registered in RegisteredWidgetStackMap"), *WidgetStackTag.ToString()));
        }
    }
}
