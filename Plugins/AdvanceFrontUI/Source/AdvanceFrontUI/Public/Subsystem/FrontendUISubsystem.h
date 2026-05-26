// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <Widgets/Widget_ActivatableBase.h>
#include "FrontendTypes/FrontendEnumType.h"
#include "FrontendUISubsystem.generated.h"



class UWidget_PrimaryLayout;
struct FGameplayTag;
class UFrontendCommonButtonBase;

/**
 * 异步推送控件时的状态枚举
 * 用于回调函数中标识当前所处的阶段
 */
enum class EAsyncPushWidgetState : uint8
{
    OnCreatedBeforePush,   // 控件已创建，但在推入堆栈之前
    AfterPush              // 控件已成功推入堆栈之后
};

//多播委托，
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdatedDelegate, UFrontendCommonButtonBase*, Button, FText, DescriptionText);


/**
 * 前端 UI 子系统（游戏实例子系统）
 * 负责管理整个前端界面的核心控件，如主布局控件的注册与查找、异步控件推送等功能
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    /**
     * 获取前端 UI 子系统实例的静态方法
     * @param WorldContextObject 用于获取世界上下文的对象（任意 UObject 指针）
     * @return 当前游戏实例下的前端 UI 子系统指针，不存在则返回 nullptr
     */
    static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

    // USubsystem 接口开始
    /**
     * 决定是否创建该子系统的钩子函数
     * @param Outer 外部对象（通常为 GameInstance）
     * @return 返回 true 表示需要创建此子系统，false 表示不创建
     */
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
    // USubsystem 接口结束

    /**
     * 注册已创建的主布局控件
     * 子系统会持有该控件引用，便于后续查找和使用
     * @param InPrimaryLayoutWidget 主布局控件的指针
     */
    UFUNCTION(BlueprintCallable)
    void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InPrimaryLayoutWidget);

    /**
     * 异步推送软引用的控件类到指定的控件堆栈中
     * @param WidgetStackTag         目标控件堆栈的标签（如 "Frontend.WidgetStack.Main"）
     * @param SoftWidgetClassPtr     软引用的控件类（TSoftClassPtr<UWidget_ActivatableBase>）
     * @param AsyncPushStateCallBack 异步推送过程中的状态回调，参数为 (状态枚举, 最终生成的控件指针)
     */
    void PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag,
        TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClassPtr,
        TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallBack);


    void PushConfirmScreenToModelStackAsync(
        EConfirmScreenType ConfirmScreenType,const FText& ScreenTitle, 
        const FText& ScreenMessage, TFunction<void(EConfirmScreenButtonType)> InButtonClickedCallback);


    /** 当按钮描述文本更新时广播的委托，参数为更新的按钮和新的描述文本 */
    UPROPERTY(BlueprintAssignable)
    FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;

private:
    /** 已创建的主布局控件指针（不参与序列化，运行时临时持有） */
    UPROPERTY(Transient)
    UWidget_PrimaryLayout* CreatedPrimaryLayoutWidget;
};