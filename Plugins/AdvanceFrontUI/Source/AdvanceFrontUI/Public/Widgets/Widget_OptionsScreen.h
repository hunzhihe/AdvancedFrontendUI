// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_OptionsScreen.generated.h"


class UOptionsDataRegistry;
class UFrontendTabListWidgetBase;
class UFrontendUICommonListView;
class UWidget_OptionsDetailView;
class UListDataObject_Base;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	protected:

	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
    //End UUserWidget Interface


	//Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
    //end  UCommonActivatableWidget Interface

	private:

	    UOptionsDataRegistry* GetOrCreateDataRegistry();

	    void OnResetBoundActionTriggered();
		void OnBackBoundActionTriggered();

		UFUNCTION()
		void OnOptionsTabSelected(FName TabID);

		void OnListViewItemHovered(UObject* InHoveredItem,bool bWasHovered);

		void OnListViewItemSelectionChanged(UObject* InSelectedItem);


		FString TryGetEntryWidgetClassName(UObject* InOwningListItem)const;

		void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsLsitDataModifyReason ModeifyReason);

		//Bound Widgets
		UPROPERTY(meta = (BindWidget))
		UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;

		UPROPERTY(meta = (BindWidget))
		UFrontendUICommonListView* CommonListView_OptionsList;

		UPROPERTY(meta = (BindWidget))
		UWidget_OptionsDetailView* DetailView_ListEntryInfo;

		//BoundWidgets

		UPROPERTY(Transient)
		UOptionsDataRegistry* CreatedOwningDataRegistry;

		UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
		FDataTableRowHandle ResetToDefaultAction;

        FUIActionBindingHandle ResetToDefaultActionHandle;


		UPROPERTY(Transient)
		TArray<UListDataObject_Base*> ResettableDataArray;

		bool bIsResettingData = false;

};
