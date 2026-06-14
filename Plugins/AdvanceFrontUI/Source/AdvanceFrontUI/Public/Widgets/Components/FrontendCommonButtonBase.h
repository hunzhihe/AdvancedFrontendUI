// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"


class UCommonTextBlock;
class UFrontendUIButtonInternalBase;
class SCommonButton;
class UCommonLazyImage;
/**
 * 前端通用按钮基类
 * 继承自 UCommonButtonBase，增加了文本/图片设置、自动大写转换、
 * 以及悬停时通过 UFrontendUISubsystem 广播描述文本的功能。
 * 使用自定义的 UFrontendUIButtonInternalBase 作为内部按钮实现。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
    public:
	/** 设置按钮显示文本，支持自动大写转换 */
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText NewText);

	/** 获取按钮当前的显示文本 */
	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	/** 设置按钮显示的图标/图片 */
	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);


private:

	//UUserWidget Interface
	virtual void NativePreConstruct() override;
	//UUserWidget Interface

	//UCommonButtonBase Interface
	/** 构造自定义内部按钮（UFrontendUIButtonInternalBase） */
	virtual UCommonButtonInternalBase* ConstructInternalButton() override;
	/** 当前文本样式变化时同步更新按钮文本样式 */
    virtual void NativeOnCurrentTextStyleChanged() override;
	/** 悬停时广播描述文本到 UFrontendUISubsystem */
	virtual void NativeOnHovered() override;
	/** 取消悬停时清除描述文本 */
	virtual void NativeOnUnhovered() override;

    //UCommonButtonBase Interface

	//Bound Widgets — 由蓝图绑定的子控件
    UPROPERTY(BlueprintReadWrite,meta = (BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;
	//Bound Widgets

	/** 按钮初始显示文本（设计态预览用） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	/** 是否将按钮文本转为大写 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
    bool bUserUpperCaseForButtonText = false;

	/** 按钮的描述文本（悬停时广播） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
    FText ButtonDiscriptionText;

	/** 缓存的内部按钮实例 */
	UPROPERTY(Transient)
	TObjectPtr<UFrontendUIButtonInternalBase> CachedInternalButton;

};
