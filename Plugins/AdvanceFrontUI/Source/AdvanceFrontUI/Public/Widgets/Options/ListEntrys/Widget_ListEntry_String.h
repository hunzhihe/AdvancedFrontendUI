// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"


class UFrontendCommonButtonBase;
class UFrontendCommonRotator;
class UListDataObject_String;
/**
 * 字符串（Rotator）列表条目控件
 * 显示左右箭头按钮 + Rotator 控件，用于在多个字符串选项之间切换。
 * 支持手柄和鼠标/键盘输入。
 */
UCLASS(Abstract,BlueprintType,meta = (DisableNaiveTick))
class ADVANCEFRONTUI_API UWidget_ListEntry_String : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:

	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//End UUserWidget Interface



	//Begin UWidget_ListEntry_Base interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject) override;
	virtual void OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason  ModifyReason) override;
	virtual void OnToggleEditableState(bool bIsEditable) override;
	//End UWidget_ListEntry_Base interface


private:

	/** 点击"上一个选项"按钮 */
	void OnPreviousOptionButtonClicked();
	/** 点击"下一个选项"按钮 */
	void OnNextOptionButtonClicked();

	/** Rotator 值变化回调（仅用户主动操作时生效） */
	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);

	//Bound Widgets — 由蓝图绑定的子控件
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_NextOption;
	//Bound Widgets

	/** 缓存的所属字符串数据对象 */
	UPROPERTY(Transient)
	UListDataObject_String* CachedOwningStringDataObject;

};
