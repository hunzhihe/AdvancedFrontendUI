// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"


void UListDataObject_String::AddDynamicOption(const FString& OptionString, const FText& OptionDisplayText)
{
	AvaiableOptionsStringArray.Add(OptionString);
	AvaiableOptionsDisplayTextArray.Add(OptionDisplayText);
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (AvaiableOptionsStringArray.IsEmpty() || AvaiableOptionsDisplayTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvaiableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndex = (CurrentDisplayIndex + 1) % AvaiableOptionsStringArray.Num();

	const bool bIsNaxtIndexValid = AvaiableOptionsStringArray.IsValidIndex(NextIndex);

	if (bIsNaxtIndexValid)
	{
		CurrentStringValue = AvaiableOptionsStringArray[NextIndex];	
	}
	else
	{
		CurrentStringValue = AvaiableOptionsStringArray[0];	
	}
	TrySetDisplayTextByStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}	
}

void UListDataObject_String::BackToPerviousOption()
{
	if (AvaiableOptionsStringArray.IsEmpty() || AvaiableOptionsDisplayTextArray.IsEmpty())
	{
		return;
	}
	const int32 CurrentDisplayIndex = AvaiableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndex = (CurrentDisplayIndex - 1 + AvaiableOptionsStringArray.Num()) % AvaiableOptionsStringArray.Num();
	const bool bIsPreviousIndexValid = AvaiableOptionsStringArray.IsValidIndex(PreviousIndex);
	if (bIsPreviousIndexValid)
	{
		CurrentStringValue = AvaiableOptionsStringArray[PreviousIndex];
	}
	else
	{
		CurrentStringValue = AvaiableOptionsStringArray.Last();
	}
	TrySetDisplayTextByStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvaiableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvaiableOptionsStringArray[0];
	}

	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	if (!TrySetDisplayTextByStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(CurrentStringValue);
	};
}

bool UListDataObject_String::TrySetDisplayTextByStringValue(const FString& InStringValue)
{
	const int32 Index = AvaiableOptionsStringArray.IndexOfByKey(InStringValue);
	if (Index != INDEX_NONE)
	{
		//CurrentStringValue = InStringValue;
		CurrentDisplayText = AvaiableOptionsDisplayTextArray[Index];
		return true;
	}

	return false;
}
