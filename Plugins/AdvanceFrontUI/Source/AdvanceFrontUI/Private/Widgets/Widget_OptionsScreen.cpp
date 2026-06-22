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

    // 注册"重置为默认值"UI 操作绑定
    if (!ResetToDefaultAction.IsNull())
    {
        ResetToDefaultActionHandle = RegisterUIActionBinding(
        FBindUIActionArgs(
            ResetToDefaultAction,
            true,
            FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered))
       );
    }

    // 注册"返回"UI 操作绑定（使用 CommonUI 默认返回操作）
    RegisterUIActionBinding(
        FBindUIActionArgs(
            ICommonInputModule::GetSettings().GetDefaultBackAction(),
            true,
            FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered))
       );

	// 绑定选项卡选择事件
	TabListWidget_OptionsTabs->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);

	// 绑定列表项悬停和选中事件
    CommonListView_OptionsList->OnItemIsHoveredChanged().AddUObject(this, &ThisClass::OnListViewItemHovered);
    CommonListView_OptionsList->OnItemSelectionChanged().AddUObject(this, &ThisClass::OnListViewItemSelectionChanged);

    //绑定语言更新事件
    UFrontendUISubsystem* UISubsystem = UFrontendUISubsystem::Get(this);
    UISubsystem->OnRegistryNewOptionsData.BindUObject(this, &ThisClass::RefreshLocalizedDataRegistry);
}

void UWidget_OptionsScreen::NativeOnActivated()
{

    Super::NativeOnActivated();

    // 遍历所有选项卡集合并动态注册到 TabListWidget
    for(UListDataObject_Collection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
    {
       if(!TabCollection)
       {
          continue;
       }
       const FName TabID = TabCollection->GetDataID();
       // 避免重复注册
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

	// 停用时保存所有设置
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

//void UWidget_OptionsScreen::UpdateDataRegistry()
//{
//    if (!CreatedOwningDataRegistry)
//    {
//        CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
//        //CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
//    }
//    else
//    {
//        CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
//    }
//}

void UWidget_OptionsScreen::RefreshLocalizedDataRegistry()
{
    // 确保 DataRegistry 已创建
    if (!CreatedOwningDataRegistry)
    {
        return;
    }

    // 刷新所有数据对象的本地化文本（原地更新，不重建对象）
    CreatedOwningDataRegistry->RefreshAllLocalizedText();

    // 刷新选项卡按钮的显示文本
    for (UListDataObject_Collection* TabCollection : CreatedOwningDataRegistry->GetRegisteredOptionsTabCollections())
    {
        if (!TabCollection)
        {
            continue;
        }
        if (UCommonButtonBase* TabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabCollection->GetDataID()))
        {
            if (UFrontendCommonButtonBase* FrontendButton = Cast<UFrontendCommonButtonBase>(TabButton))
            {
                FrontendButton->SetButtonText(TabCollection->GetDataDisplayName());
            }
        }
    }

    // 遍历当前列表中所有已创建的条目控件，强制其从数据对象中重新读取本地化文本
    // 不依赖 SetListItems + RequestRefresh 重建，因为相同对象指针可能被 ListView 优化跳过
    const TArray<UObject*>& ListItems = CommonListView_OptionsList->GetListItems();
    for (UObject* Item : ListItems)
    {
        if (UUserWidget* EntryWidget = CommonListView_OptionsList->GetEntryWidgetFromItem(Item))
        {
            if (UWidget_ListEntry_Base* EntryBase = Cast<UWidget_ListEntry_Base>(EntryWidget))
            {
                // 触发子类重新从数据对象读取显示文本（如 Rotator 标签、选中项文本）
                EntryBase->OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(Item));
            }
        }
    }

    // 同时刷新详情视图中的描述文本
    if (UListDataObject_Base* SelectedItem = CommonListView_OptionsList->GetSelectedItem<UListDataObject_Base>())
    {
        DetailView_ListEntryInfo->UpdateDetailView(SelectedItem, TryGetEntryWidgetClassName(SelectedItem));
    }
}


void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
    //FrontendUIDebugHelper::Log("Reset to default action triggered.");

	// 无重置项时直接返回
    if (ResettableDataArray.IsEmpty())
    {
        return;
    }

	// 获取当前活动选项卡按钮的显示名称
    UCommonButtonBase* SelectedTabButton = TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabListWidget_OptionsTabs->GetActiveTab());

    FString TabButtonDisplayName =  CastChecked<UFrontendCommonButtonBase>(SelectedTabButton)->GetButtonDisplayText().ToString();

	// 弹出确认对话框，确认后执行重置
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
                    //FrontendUIDebugHelper::Log(DataToReset->GetDataDisplayName().ToString() + TEXT("was rest"));
                }
                else
                {
                    bHasDataFailedToReset = true;
                    //FrontendUIDebugHelper::Log(DataToReset->GetDataDisplayName().ToString() + TEXT("failed to reset"));
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

	// 切换选项卡时清空详情视图
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

	// 重新构建可重置数据列表
    ResettableDataArray.Empty();

    for (UListDataObject_Base* FoundListSourceItem : FoundListSourceItems)
    {
        if (!FoundListSourceItem) {
            continue;
        }

		// 绑定数据修改回调
        FoundListSourceItem->OnListDataModified.AddUObject(this,
            &ThisClass::OnListViewListDataModified);

		// 收集所有可重置的数据对象
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
	// 重置过程中忽略修改通知
    if (!ModifiedData || bIsResettingData)
    {
        return;
    }
    if (ModifiedData->CanResetBackToDefaultValue())
    {
    	// 数据变为可重置状态时添加到列表并确保操作绑定激活
        ResettableDataArray.AddUnique(ModifiedData);

        if (!GetActionBindings().Contains(ResetToDefaultActionHandle))
        {
            AddActionBinding(ResetToDefaultActionHandle);
        }
    }
    else
    {
    	// 不可重置时从列表中移除
        if (ResettableDataArray.Contains(ModifiedData))
        {
            ResettableDataArray.Remove(ModifiedData);
        }

    }
    // 无重置项时移除操作绑定
    if (ResettableDataArray.IsEmpty())
    {
        RemoveActionBinding(ResetToDefaultActionHandle);
    }

}


