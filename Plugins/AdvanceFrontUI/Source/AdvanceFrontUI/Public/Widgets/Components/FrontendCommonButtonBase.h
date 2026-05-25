// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"


class UCommonTextBlock;
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

	private:
    
	//UUserWidget Interface
	virtual void NativePreConstruct() override;
	//UUserWidget Interface

	//UCommonButtonBase Interface
    virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
    //UCommonButtonBase Interface

	//Bound Widgets
    UPROPERTY(meta = (BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;
	//Bound Widgets

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
    bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Common Button", meta = (AllowPrivateAccess = "true"))
    FText ButtonDiscriptionText;
};
