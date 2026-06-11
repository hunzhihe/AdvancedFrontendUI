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
	//SetVisibility(ESlateVisibility::Visible);

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
	if (CommonText_SettingDisplayName)
	{
		CommonText_SettingDisplayName->SetText(InListDataObject->GetDataDisplayName());
	}

	if (!InListDataObject->OnListDataModified.IsBoundToObject(this))
	{
		InListDataObject->OnListDataModified.AddUObject(
			this,
			&UWidget_ListEntry_Base::OnOwingListDataObjectModifed
		);
	}

	if (!InListDataObject->OnDependencyDataModified.IsBoundToObject(this))
	{
		InListDataObject->OnDependencyDataModified.AddUObject(
			this,
			&ThisClass::OnOwningDependencyDataObjectModified

		);
	}


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
