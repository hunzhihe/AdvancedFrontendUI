// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListDataObject_Base.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptinsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InDependencyData)
{
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(
			this,
			&ThisClass:: OnEditDependencyDataModifiied
			);
	}
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	for (const FOptinsDataEditConditionDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;


		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		if (Condition.HasForcedStringVALUE())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();
			
			//if the current value this data object has can be set to the froced value
			if (CanSetToForcedStringvalue(ForcedStringValue))
			{
				OnSetToForcedStringvalue(ForcedStringValue);
			}

		}
	}
	return bIsEditable;

}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* InModifiedListDataObject, EOptionsLsitDataModifyReason ModifyReason)
{
	OnListDataModified.Broadcast(InModifiedListDataObject, ModifyReason);

	if (bShouldApplyChangeImmediatly)
	{
		UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->ApplySettings(true);
	}
}

void UListDataObject_Base::OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason)
{

	OnDependencyDataModified.Broadcast(InModifiedDependencyData, ModifyReason);
}
