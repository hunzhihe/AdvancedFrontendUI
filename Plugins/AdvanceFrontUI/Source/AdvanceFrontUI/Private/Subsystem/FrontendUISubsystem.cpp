// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendUIDebugHelper.h"
#include "Engine/AssetManager.h"
#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widgets/Widget_ConformScreen.h"
#include "FrontendGameplayTags.h"
#include "FrontendUIFunctionLibrary.h"



/**
 * 静态方法：获取前端 UI 子系统实例
 * @param WorldContextObject 用于获取世界的上下文对象
 * @return 子系统指针，获取失败返回 nullptr
 */
UFrontendUISubsystem* UFrontendUISubsystem::Get(const UObject* WorldContextObject)
{
    // 检查 GEngine 是否存在（确保引擎已初始化）
    if (GEngine)
    {
        // 从上下文对象中获取世界（断言模式：失败直接报错）
        UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
        // 通过世界的游戏实例获取该子系统
        return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
    }
    return nullptr;
}

/**
 * 决定是否创建该子系统
 * @param Outer 外部对象，此处应为 UGameInstance
 * @return 返回 true 表示需要创建，false 表示不创建
 */
bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    // 如果不是专用服务器实例（即客户端或独立游戏）
    if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
    {
        // 收集所有派生自 UFrontendUISubsystem 的类
        TArray<UClass*> FoundClasses;
        GetDerivedClasses(UFrontendUISubsystem::StaticClass(), FoundClasses, true);

        // 如果存在多个派生类，则警告并禁止创建当前子系统（避免多实例冲突）
        if (FoundClasses.Num() > 1)
        {
            UE_LOG(LogTemp, Warning, TEXT("Multiple classes derived from UFrontendUISubsystem found. Subsystem will not be created. Please ensure only one class derives from UFrontendUISubsystem."));
            return false;
        }
        return true;
    }
    // 专用服务器上不创建 UI 子系统
    return false;
}

/**
 * 注册已创建的主布局控件
 * 子系统会持有该控件的引用，供后续推送控件时查找堆栈容器使用
 * @param InPrimaryLayoutWidget 主布局控件指针
 */
void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InPrimaryLayoutWidget)
{
    if (InPrimaryLayoutWidget)
    {
        // 保存主布局控件引用
        CreatedPrimaryLayoutWidget = InPrimaryLayoutWidget;
        // 输出调试日志（通过自定义辅助类）
        //FrontendUIDebugHelper::Log(FString::Printf(TEXT("Registered Primary Layout Widget: %s"), *InPrimaryLayoutWidget->GetName()));
    }
}

/**
 * 异步推送软引用控件类到指定的控件堆栈
 * @param WidgetStackTag          目标控件堆栈的标签（用于在主布局中查找对应的容器）
 * @param SoftWidgetClassPtr      软引用的控件类（必须继承自 UWidget_ActivatableBase）
 * @param AsyncPushStateCallBack  异步推送回调，参数：(状态, 生成的控件实例)
 */
void UFrontendUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag,
    TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClassPtr,
    TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallBack)
{
    // 确保软引用类有效
    if (!SoftWidgetClassPtr.IsNull())
    {
        // 使用资产管理器的流式管理器异步加载该类
        UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
            SoftWidgetClassPtr.ToSoftObjectPath(),
            FStreamableDelegate::CreateLambda([this, WidgetStackTag, SoftWidgetClassPtr, AsyncPushStateCallBack]()
                {
                    // 加载完成后获取实际的 UClass 指针
                    UClass* LoadedClass = SoftWidgetClassPtr.Get();
                    // 确认类加载成功且主布局控件已注册
                    if (LoadedClass && CreatedPrimaryLayoutWidget)
                    {
                        //FrontendUIDebugHelper::Log(FString::Printf(TEXT("Successfully loaded widget class: %s"), *LoadedClass->GetName()));

                        // 根据标签查找对应的控件堆栈容器
                        UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayoutWidget->FindRegisteredWidgetStackByTag(WidgetStackTag);

                        // 向容器中添加控件（AddWidget 内部会创建控件实例）
                        // 传入一个回调，用于在控件创建后、推入堆栈前通知调用方（OnCreatedBeforePush 状态）
                        UWidget_ActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
                            LoadedClass,
                            [AsyncPushStateCallBack](UWidget_ActivatableBase& CreatedWidgetInstance) {
                                AsyncPushStateCallBack(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
                            }
                        );

                        // 推送完成后再次回调（AfterPush 状态），传递最终生成的控件指针
                        AsyncPushStateCallBack(EAsyncPushWidgetState::AfterPush, CreatedWidget);
                    }
                    else
                    {
                        // 加载失败或主布局缺失时输出警告
                        FrontendUIDebugHelper::Log(FString::Printf(TEXT("Failed to load widget class for tag: %s"), *WidgetStackTag.ToString()));
                    }
                })
        );
    }
}

void UFrontendUISubsystem::PushConfirmScreenToModelStackAsync(EConfirmScreenType ConfirmScreenType, const FText &ScreenTitle, const FText &ScreenMessage, TFunction<void(EConfirmScreenButtonType)> InButtonClickedCallback)
{
    UConfirmScreenInfoObject* CreatedInfoObject = nullptr;


    switch (ConfirmScreenType)
    {
    case EConfirmScreenType::OK:
        CreatedInfoObject= UConfirmScreenInfoObject::CreateOKScreen(
            ScreenTitle, ScreenMessage);
        break;
    case EConfirmScreenType::OkCancel:
        CreatedInfoObject= UConfirmScreenInfoObject::CreateOKCancelScreen(
            ScreenTitle, ScreenMessage);
        break;
    case EConfirmScreenType::YesNo:
        CreatedInfoObject= UConfirmScreenInfoObject::CreateYesNoScreen(
            ScreenTitle, ScreenMessage);
        break;
    case EConfirmScreenType::Unknown:
        break;    

    default:
        break;
    }

    check(CreatedInfoObject);
    PushSoftWidgetToStackAsync(
        FrontendGameplayTags::Frontend_WidgetStack_Modal,
        UFrontendUIFunctionLibrary::GetFrontendWidgetFromTag(FrontendGameplayTags::Frontend_Widget_ConfirmScreen),
        [ CreatedInfoObject, InButtonClickedCallback](EAsyncPushWidgetState AsyncState, UWidget_ActivatableBase* CreatedWidget)
        {
            if (AsyncState == EAsyncPushWidgetState::AfterPush)
            {
                if (UWidget_ConformScreen* ConfirmScreenWidget = Cast<UWidget_ConformScreen>(CreatedWidget))
                {
                    ConfirmScreenWidget->InitConformScreen(
                        CreatedInfoObject,InButtonClickedCallback
                    );
                }
            }
        }
    );

}
    