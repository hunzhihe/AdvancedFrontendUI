// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "FrontendUIDebugHelper.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Components/FrontendUICommonListView.h"


void UWidget_OptionsScreen::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (!ResetToDefaultAction.IsNull())
    {
        ResetToDefaultActionHandle = RegisterUIActionBinding(
        FBindUIActionArgs(
            ResetToDefaultAction,
            true,
            FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered))
       );
    }

    //ICommonInputModule::GetSettings().SetDefaultBackAction(true);

    RegisterUIActionBinding(
        FBindUIActionArgs(
            ICommonInputModule::GetSettings().GetDefaultBackAction(),
            true,
            FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered))
       );

	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
    
   
}

void UWidget_OptionsScreen::NativeOnActivated()
{

    Super::NativeOnActivated();

    for(UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
    {
       if(!TabCollection)
       {
          continue;
       }
       const FName TabID = TabCollection->GetDataID();
       if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabID)!=nullptr)
       {
           continue;
       }
       TabListWidget_OptionsTabs->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
    } 
    


}
UOptionsDataRegistry *UWidget_OptionsScreen::GetOrCreateDataRegistry()
{
    if(!CreatedOwningDataRegistry)
    {
       CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
       CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
    }


    return CreatedOwningDataRegistry;
}


void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
    FrontendUIDebugHelper::Log("Reset to default action triggered.");
}
void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
    DeactivateWidget();
    FrontendUIDebugHelper::Log("Back action triggered on options screen.");
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{
    //FrontendUIDebugHelper::Log("Options tab selected: " + TabID.ToString());
	TArray<UListDataObject_Base*> FoundListSourceItems = 
        GetOrCreateDataRegistry()->GetListSourceItemsBySelectedTabID(TabID);

	CommonListView_OptionsList->SetListItems(FoundListSourceItems);
    CommonListView_OptionsList->RequestRefresh();

    if (CommonListView_OptionsList->GetNumItems() !=0)
    {
        CommonListView_OptionsList->NavigateToIndex(0);
		CommonListView_OptionsList->SetSelectedIndex(0);
    }

}
