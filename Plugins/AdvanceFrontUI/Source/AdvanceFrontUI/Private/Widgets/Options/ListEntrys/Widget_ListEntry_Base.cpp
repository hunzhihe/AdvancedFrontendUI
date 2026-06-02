// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "CommonTextBlock.h"

void UWidget_ListEntry_Base::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	OnOwningListDataObjectSet(CastChecked<UListDataObject_Base>(ListItemObject));

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
}

void UWidget_ListEntry_Base::OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason ModifyReason)
{

}
