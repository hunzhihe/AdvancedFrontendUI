// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"


void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvaiableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvaiableOptionsStringArray[0];
	}

	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefualtValueAsString();
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
		CurrentDisplayText = FText::FromString("Invalid Options");
	};
}

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

void UListDataObject_String::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AvaiableOptionsDisplayTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool 
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	if (FoundIndex != INDEX_NONE && AvaiableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		CurrentDisplayText = InNewSelectedText;
		CurrentStringValue = AvaiableOptionsStringArray[FoundIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			NotifyListDataModified(this);
		}
	}
}



bool UListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefualtValueAsString();
}

bool UListDataObject_String::TryResetBackToDefaultValue() 
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefualtValueAsString();
		TrySetDisplayTextByStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			NotifyListDataModified(this, EOptionsLsitDataModifyReason::ResetToDefault);

			return true;

		}
	}
	return false;
}

bool UListDataObject_String::CanSetToForcedStringvalue(const FString& InForcedValue) const
{
	return CurrentStringValue != InForcedValue;
}

void UListDataObject_String::OnSetToForcedStringvalue(const FString& InForcedValue)
{
	CurrentStringValue = InForcedValue;
	TrySetDisplayTextByStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this, EOptionsLsitDataModifyReason::DependencyModified);
	}
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


//void UListDataObject_Stringbool::OverrideTrueDisplayText(const FText& InNameTrueDisplayText)
//{
//	if (!AvaiableOptionsStringArray.Contains(TrusString))
//	{
//		AddDynamicOption(TrusString, InNameTrueDisplayText);
//	}
//}
//
//void UListDataObject_Stringbool::OverrideFalseDisplayText(const FText& InNameFalseDisplayText)
//{
//	if (!AvaiableOptionsStringArray.Contains(FalseString))
//	{
//		AddDynamicOption(FalseString, InNameFalseDisplayText);
//	}
//}
//
//void UListDataObject_Stringbool::SetTrueAsDefaultValue()
//{
//	SetDefaultValueFromString(TrusString);
//}
//
//void UListDataObject_Stringbool::SetFalseAsDefaultValue()
//{
//	SetDefaultValueFromString(FalseString);
//}
//
//void UListDataObject_Stringbool::OnDataObjectInitialized()
//{
//	TryInitBoolValue();
//
//	Super::OnDataObjectInitialized();
//}
//
////UListDataObject_Stringbool
//void UListDataObject_Stringbool::TryInitBoolValue()
//{
//	if (!AvaiableOptionsStringArray.Contains(TrusString))
//	{
//		AddDynamicOption(TrusString, FText::FromString(TEXT("ON")));
//	}
//
//	if (!AvaiableOptionsStringArray.Contains(FalseString))
//	{
//		AddDynamicOption(FalseString, FText::FromString(TEXT("OFF")));
//	}
//
//}


//UListDataObject_Stringbool