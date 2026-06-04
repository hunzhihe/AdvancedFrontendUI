// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_OptionsScreen.h"
#include "Input/CommonUIInputTypes.h"
#include "ICommonInputModule.h"
#include "FrontendUIDebugHelper.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Widgets/Components/FrontendUICommonListView.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widgets/Options/Widget_OptionsDetailView.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

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
    
    CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
    CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelectionChanged);
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
void UWidget_OptionsScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->ApplySettings(true);
	
}
UWidget* UWidget_OptionsScreen::NativeGetDesiredFocusTarget() const
{
    if (UObject* SelectedObject = CommonListView_OptionsList->GetSelectedItem())
    {
        if (UUserWidget* SelectedEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(SelectedObject))
        {
            return SelectedEntryWidget;
        }
     }

    return Super::NativeGetDesiredFocusTarget();
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
    //FrontendUIDebugHelper::Log("Reset to default action triggered.");

    if (ResettableDataArray.IsEmpty())
    {
        return;
    }

    UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());

    FString TabButtonDisplayName =  CastChecked<UFrontendCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

    UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
        EConfirmScreenType::YesNo,
        FText::FromString(TEXT("Reset")),
        FText::FromString(TEXT("Are you sure you want to reset all the srtting under the") + TabButtonDisplayName + TEXT("tcb.")),
        [this](EConfirmScreenButtonType ClickedButtonType) {

            if (ClickedButtonType != EConfirmScreenButtonType::Confirmed )
            {
                return;
            }

            bIsResettingData = true;
            bool bHasDataFailedToReset = false;

            for (UListDataObject_Base* DataToReset : ResettableDataArray)
            {
                if (!DataToReset)
                {
                    continue;
                }

                if (DataToReset->TryResetBackToDefaultValue())
                {
                    FrontendUIDebugHelper::Log(DataToReset->GetDataDisplayName().ToString() + TEXT("was rest"));
                }
                else
                {
                    bHasDataFailedToReset = true;
                    FrontendUIDebugHelper::Log(DataToReset->GetDataDisplayName().ToString() + TEXT("failed to reset"));
                }
            }


            if (!bHasDataFailedToReset)
            {
                ResettableDataArray.Empty();

                RemoveActionBinding(ResetToDefaultActionHandle);
            }

            bIsResettingData = false;

        }


    );

}
void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
    DeactivateWidget();
    //FrontendUIDebugHelper::Log("Back action triggered on options screen.");
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{

    DetailView_ListEntryInfo->ClearDetailView();

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

    ResettableDataArray.Empty();

    for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
    {
        if (!FoundListSourceItem) {
            continue;
        }

        FoundListSourceItem->OnListDataModified.AddUObject(this,
            &ThisClass::OnListViewListDataModified);

        if (FoundListSourceItem->CanResetBackToDefaultValue())
        {
            ResettableDataArray.AddUnique(FoundListSourceItem);
        }
    }
    if (ResettableDataArray.IsEmpty())
    {
        RemoveActionBinding(ResetToDefaultActionHandle);
    }
    else
    {
        if (!GetActionBindings().Contains(ResetToDefaultActionHandle))
        {
            AddActionBinding(ResetToDefaultActionHandle);
        }
    }
}

void UWidget_OptionsScreen::OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered)
{
    if (!InHoveredItem)
    {
        return;
    }
    UWidget_ListEntry_Base* HoveredEntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem<UWidget_ListEntry_Base>(InHoveredItem);

    check(HoveredEntryWidget);

	HoveredEntryWidget->NativeOnListEntryWidgetHovered(bWasHovered);

    if (bWasHovered)
    {
        DetailView_ListEntryInfo->UpdateDetailView(
            CastChecked<UListDataObject_Base>(InHoveredItem),
            TryGetEntryWidgetClassName(InHoveredItem)
        );
    }
    else
    {
        if (UListDataObject_Base* SelectItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
        {
            DetailView_ListEntryInfo->UpdateDetailView(
                SelectItem,
                TryGetEntryWidgetClassName(SelectItem)
            );
        }
    }
    //测试
	//FrontendUIDebugHelper::Log("List item " + InHoveredItem->GetName() + (bWasHovered ? " hovered." : " unhovered."));
}

void UWidget_OptionsScreen::OnListViewItemSelectionChanged(UObject* InSelectedItem)
{
    if (!InSelectedItem)
    {
        return;
    }

    DetailView_ListEntryInfo->UpdateDetailView(
        CastChecked<UListDataObject_Base>(InSelectedItem),
        TryGetEntryWidgetClassName(InSelectedItem)
    );
    //测试
	//FrontendUIDebugHelper::Log("List item " + InSelectedItem->GetName() + " selected.");
}

FString UWidget_OptionsScreen::TryGetEntryWidgetClassName(UObject* InOwningListItem) const
{
    if (UUserWidget* FoundEnrtyWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(InOwningListItem))
    {
        return FoundEnrtyWidget->GetClass()->GetName();
    }

    return TEXT("Entry Widget Not Valid");
}

void UWidget_OptionsScreen::OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsLsitDataModifyReason ModeifyReason)
{
    if (!ModifiedData || bIsResettingData)
    {
        return;
    }if (ModifiedData->CanResetBackToDefaultValue())
    {
        ResettableDataArray.AddUnique(ModifiedData);

        if (!GetActionBindings().Contains(ResetToDefaultActionHandle))
        {
            AddActionBinding(ResetToDefaultActionHandle);
        }
    }
    else
    {
        if (ResettableDataArray.Contains(ModifiedData))
        {
            ResettableDataArray.Remove(ModifiedData);
        }

    }
    if (ResettableDataArray.IsEmpty())
    {
        RemoveActionBinding(ResetToDefaultActionHandle);
    }

}


