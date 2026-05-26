// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_PushConfirmScreen.generated.h"

//多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);
/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UAsyncAction_PushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	public:
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirm Screen"), Category = "Advance Frontend UI|Async")
	static UAsyncAction_PushConfirmScreen* PushConfirmScreen(
		UObject* WorldContextObject,
		FText ScreenTitle,
		FText ScreenMessage,
		EConfirmScreenType ConfirmScreenType);

	//激活函数，开始执行异步操作
	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked; // 蓝图可绑定的事件，当按钮被点击时触发

	private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedConfirmScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
};
