// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_ListEntry_Base.generated.h"


class UCommonTextBlock;
class UListDataObject_Base;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered,bool bIsEntryWidgetStillSelected);
    void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "GetWidgetToFocusForGamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;

	//Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	//End IUserObjectListEntry Interface

	//Begin UUserWidget Interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//End UUserWidget Interface

	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject);

	virtual void OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason  ModifyReason);

	virtual void OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependencyData, EOptionsLsitDataModifyReason  ModifyReason);

	virtual void OnToggleEditableState(bool bIsEditable);


	void SelectThisListEntryWidget();
private:
	//Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//Bound Widgets
	
	UPROPERTY(Transient)
	UListDataObject_Base* CachedOwningDataObject;


};
