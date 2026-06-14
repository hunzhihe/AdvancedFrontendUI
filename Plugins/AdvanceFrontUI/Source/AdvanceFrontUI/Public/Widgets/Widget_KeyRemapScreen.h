// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "CommonInputTypeEnum.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;
/**
 * 按键重映射界面控件
 * 激活后通过自定义输入预处理器拦截所有按键输入，
 * 根据目标输入类型（键盘鼠标/手柄）过滤并捕获有效按键，
 * 完���后通过委托将结果传递给调用方。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

public:
	/** 设置需要过滤的目标输入类型（键盘鼠标或手柄） */
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	/** 按键捕获成功时的委托，参数为捕获到的按键 */
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnKeyRemapScreenKeyPressedDelegate  OnKeyRemapScreenKeyPressed;

	/** 按键捕获取消时的委托，参数为取消原因字符串 */
	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCanceledDelegate, const FString& /*CanceledReason*/)
	FOnKeyRemapScreenKeySelectCanceledDelegate  OnKeyRemapScreenKeySelectCanceled;
protected:

	//Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//End UCommonActivatableWidget Interface


private:

	/** 当检测到有效按键按下时调用 */
	void OnValidKeyPressedDelected(const FKey& PressKey);
	/** 当按键选择被取消时调用（如按下 Escape） */
	void OnKeySelectCanceled(const FString& CanceledReason);

	/**
	 * 延迟一帧后停用控件，确保输入预处理正确完成
	 * @param PreDeactivateCallback 停用前执行的回调
	 */
	void RequestDeactivateWidget(TFunction<void()>PreDeactivateCallback);

	//Bound Widgets — 由蓝图绑定的富文本提示信息
	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage;
	//Bound Widgets

	/** 缓存的输入预处理器，用于拦截原始按键输入 */
	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;

	/** 缓存的目标输入类型 */
	ECommonInputType CachedDesiredInputType;
};
