// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushConfirmScreen.generated.h"

/** 确认画面按钮点击委托，参数为点击的按钮类型 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);
/**
 * 异步操作：推送确认画面到模态堆栈
 * 继承自 UBlueprintAsyncActionBase，可在蓝图异步节点中使用。
 *
 * 功能：异步显示一个确认对话框（OK / YesNo / OKCancel），
 *       等待用户点击后通过委托返回按钮类型，然后自动销毁节点。
 */
UCLASS()
class ADVANCEFRONTUI_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	public:

	/**
	 * 静态工厂方法：创建一个推送确认画面的异步操作节点
	 * @param WorldContextObject  世界上下文对象
	 * @param ScreenTitle         画面标题
	 * @param ScreenMessage       画面消息内容
	 * @param ConfirmScreenType   确认画面类型（OK/YesNo/OKCancel）
	 * @return 异步操作节点，可在蓝图中绑定 OnButtonClicked 事件
	 */
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirm Screen"), Category = "Advance Frontend UI|Async")
	static UAsyncAction_PushConfirmScreen* PushConfirmScreen(
		UObject* WorldContextObject,
		FText ScreenTitle,
		FText ScreenMessage,
		EConfirmScreenType ConfirmScreenType);

	/** 激活异步操作，开始执行 */
	virtual void Activate() override;

	/** 按钮点击时广播的事件，蓝图可绑定此事件获取用户选择结果 */
	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;

	private:
		/** 缓存的所属世界（弱引用） */
		TWeakObjectPtr<UWorld> CachedOwningWorld;
		/** 缓存的确认画面类型 */
		EConfirmScreenType CachedConfirmScreenType;
		/** 缓存的画面标题 */
		FText CachedScreenTitle;
		/** 缓存的画面消息 */
		FText CachedScreenMessage;
};
