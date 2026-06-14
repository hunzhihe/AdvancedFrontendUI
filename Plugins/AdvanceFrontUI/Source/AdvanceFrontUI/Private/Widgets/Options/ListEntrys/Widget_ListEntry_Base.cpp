// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "CommonTextBlock.h"
#include "Components/ListView.h"
#include "CommonInputSubsystem.h"

void UWidget_ListEntry_Base::NativeOnListEntryWidgetHovered(bool bWasHovered)
{
	BP_OnListEntryWidgetHovered(bWasHovered, GetListItem() ? IsListItemSelected() : false);

	if (bWasHovered)
	{
		BP_OnToggleEntryWidgetHighlightState(true);
	}
	else
	{
		BP_OnToggleEntryWidgetHighlightState(GetListItem() && IsListItemSelected() ? true : false);
	}
}

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	// 通知子类数据对象已设置
	OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));

}
void UWidget_ListEntry_Base::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
	BP_OnToggleEntryWidgetHighlightState(bIsSelected);
}
void UWidget_ListEntry_Base::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	NativeOnListEntryWidgetHovered(false);

}
FReply UWidget_ListEntry_Base::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	// 手柄模式：将焦点重定向到蓝图指定的子控件
	if (CommonInputSubsystem && CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		if (UWidget* WidgetToFocus = BP_GetWidgetToFocusForGamepad())
		{
			if (TSharedPtr<SWidget> SlateWidgetToFocus =  WidgetToFocus->GetCachedWidget())
			{
				return FReply::Handled().SetUserFocus(SlateWidgetToFocus.ToSharedRef());
			}

		}
	}
	return Super::NativeOnFocusReceived(InGeometry, InFocusEvent);


}
void UWidget_ListEntry_Base::OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject)
{
	// 设置标题文本
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InListDataObject->GetDataDisplayName());
	}

	// 绑定数据修改事件（防止重复绑定）
	if (!InListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InListDataObject->OnListDataModified.AddUObject(
			this,
			&UWidget_ListEntry_Base::OnOwingListDataObjectModifed
		);
	}

	// 绑定依赖数据修改事件
	if (!InListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		InListDataObject->OnDependencyDataModified.AddUObject(
			this,
			&ThisClass::OnOwningDependencyDataObjectModified

		);
	}

	// 初始化可编辑状态
	OnToggleEditableState(InListDataObject->IsDataCurrentlyEditable());

	CachedOwningDataObject = InListDataObject;

}

void UWidget_ListEntry_Base::OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason ModifyReason)
{
	

}

void UWidget_ListEntry_Base::OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason)
{
	if (CachedOwningDataObject)
	{
		OnToggleEditableState(CachedOwningDataObject->IsDataCurrentlyEditable());
	}
	
}

void UWidget_ListEntry_Base::OnToggleEditableState(bool bIsEditable)
{

	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetIsEnabled(bIsEditable);
	}
}

void UWidget_ListEntry_Base::SelectThisListEntryWidget()
{
	CastChecked<UListView>(GetOwningListView())->SetSelectedItem(GetListItem());
}
