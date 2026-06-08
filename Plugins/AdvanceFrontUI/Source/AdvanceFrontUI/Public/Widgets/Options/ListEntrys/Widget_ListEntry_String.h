// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_String.generated.h"


class UFrontendCommonButtonBase;
class UFrontendCommonRotator;
class UListDataObject_String;
/**
 * 
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

	void OnPreviousOptionButtonClicked();
	void OnNextOptionButtonClicked();


	void OnRotatorValueChanged(int32 Value, bool bUserInitiated);

	//Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_PreviousOption;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UFrontendCommonButtonBase* CommonButton_NextOption;
	//Bound Widgets

	UPROPERTY(Transient)
	UListDataObject_String* CachedOwningStringDataObject;
	
};
