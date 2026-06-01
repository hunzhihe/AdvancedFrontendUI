// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"


class UOptionsDataRegistry;
class UFrontendTabListWidgetBase;
class UFrontendUICommonListView;
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
    //end  UCommonActivatableWidget Interface

	private:

	    UOptionsDataRegistry* GetOrCreateDataRegistry();

	    void OnResetBoundActionTriggered();
		void OnBackBoundActionTriggered();

		UFUNCTION()
		void OnOptionsTabSelected(FName TabID);


		//Bound Widgets
		UPROPERTY(meta = (BindWidget))
		UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;

		UPROPERTY(meta = (BindWidget))
		UFrontendUICommonListView* CommonListView_OptionsList;

		//BoundWidgets

		UPROPERTY(Transient)
		UOptionsDataRegistry* CreatedOwningDataRegistry;

		UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
		FDataTableRowHandle ResetToDefaultAction;

        FUIActionBindingHandle ResetToDefaultActionHandle;
		

};
