// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"


class UFrontendCommonButtonBase;
class UListDataObject_KeyRemap;
/**
 * 按键重映射列表条目控件
 * 显示当前绑定的按键图标、重映射按钮和重置按钮。
 * 点击重映射按钮后推送 UWidget_KeyRemapScreen 到模态堆栈捕获新按键。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()

protected:
	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//End UUserWidget Interface



	//Begin UWidget_ListEntry_Base Interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject) override;

	virtual void OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason  ModifyReason) override;
	//End UWidget_ListEntry_Base Interface


private:

	/** 点击重映射按钮：推送按键捕获画面到模态堆栈 */
	void OnRemapKeyButtonClicked();
	/** 点击重置按钮：还原为默认按键绑定 */
	void OnResetKeyBindingButtonClicked();

	/** 当用户在按键捕获画面中按下有效按键时调用 */
	void OnKeyToRemapPress(const FKey& PressedKey);
	/** 当按键选择被取消时调用（如按下 Escape） */
	void OnKeyRemapCanceled(const FString& CanaceledReason);

	//Bound Widgets — 由蓝图绑定的子控件
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget,AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_RemapKey;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_ResetKeyBinding;

	/** 缓存的所属按键重映射数据对象 */
	UPROPERTY(Transient)
	UListDataObject_KeyRemap* CachedOwningKeyRemapDataObject;



};
