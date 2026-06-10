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
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
    public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText NewText);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);
	

private:
    
	//UUserWidget Interface
	virtual void NativePreConstruct() override;
	//UUserWidget Interface

	//UCommonButtonBase Interface
	// 重写以使用我们自定义的内部按钮类
	virtual UCommonButtonInternalBase* ConstructInternalButton() override;
    virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	  // 重写焦点接收事件，实现 "深度聚焦"
	//virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

    //UCommonButtonBase Interface

	//Bound Widgets
    UPROPERTY(BlueprintReadWrite,meta = (BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;
	//Bound Widgets

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
    bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
    FText ButtonDiscriptionText;


	UPROPERTY(Transient)
	TObjectPtr<UFrontendUIButtonInternalBase> CachedInternalButton;

};
