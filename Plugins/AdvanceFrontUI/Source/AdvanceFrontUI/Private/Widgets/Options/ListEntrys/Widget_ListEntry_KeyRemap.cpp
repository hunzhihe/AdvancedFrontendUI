// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntrys/Widget_ListEntry_KeyRemap.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendGameplayTags.h"
#include "FrontendUIFunctionLibrary.h"
#include "Widgets/Widget_KeyRemapScreen.h"


#include "FrontendUIDebugHelper.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::	OnResetKeyBindingButtonClicked);
	
   
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject)
{
	Super::OnOwningListDataObjectSet(InListDataObject);

	CachedOwningKeyRemapDataObject= CastChecked<UListDataObject_KeyRemap>(InListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisListEntryWidget();

	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendUIFunctionLibrary::GetFrontendWidgetFromTag(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState,UWidget_ActivatableBase* PushedWidget) {

			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen =  CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPress);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);
				if (CreatedKeyRemapScreen)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}
			
			
			}

		}


	);

	//FrontendUIDebugHelper::Log(TEXT("Remp Key Button Clicked"));
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisListEntryWidget();

	if (!CachedOwningKeyRemapDataObject)
	{
		return;
	}

	//Check if the currrent key is already the default key,Display an OK screen that says this is the default to player.
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
			EConfirmScreenType::OK,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("The Key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" is already set to Default")),
		    [](EConfirmScreenButtonType ClickedButton){}

			);
		return;
	}

	//Reset the key binding back to default
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(TEXT("Are you sure you want to reset the key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("?")),
		[this](EConfirmScreenButtonType ClickButton) {

			if (ClickButton == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}

		}

	);

	//FrontendUIDebugHelper::Log(TEXT("Reset Key Button Clicked"));
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPress(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}

	//FrontendUIDebugHelper::Log(TEXT("Valid key to remap detected,key: ") + PressedKey.GetDisplayName().ToString());
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanaceledReason)
{
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
		EConfirmScreenType::OK,
		FText::FromString(TEXT("Key Remp")),
		FText::FromString(CanaceledReason),
		[](EConfirmScreenButtonType ClickedButton) {}


	);
}
