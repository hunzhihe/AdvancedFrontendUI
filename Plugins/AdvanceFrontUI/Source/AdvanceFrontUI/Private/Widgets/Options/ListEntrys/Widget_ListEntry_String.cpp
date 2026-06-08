// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntrys/Widget_ListEntry_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonInputSubsystem.h"
#include "FrontendUIDebugHelper.h"

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnNextOptionButtonClicked);

	CommonRotator_AvailableOptions->OnClicked().AddLambda([this]()
		{
			SelectThisListEntryWidget();
		});

	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);

}

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject)
{
	Super::OnOwningListDataObjectSet(InListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvaiableOptionsDisplayTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_String::OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}

}

void UWidget_ListEntry_String::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);


	CommonButton_PreviousOption->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
	CommonButton_NextOption->SetIsEnabled(bIsEditable);
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPerviousOption();
	}
	SelectThisListEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}
	SelectThisListEntryWidget();
}

void UWidget_ListEntry_String::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningStringDataObject)
	{
		return;
	}
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChange(CommonRotator_AvailableOptions->GetSelectedText());
	}
}
