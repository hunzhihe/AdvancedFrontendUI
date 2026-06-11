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
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyBindingButtonClicked);
   
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
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendUIFunctionLibrary::GetFrontendWidgetFromTag(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState,UWidget_ActivatableBase* PushedWidget) {

			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen =  CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				if (CreatedKeyRemapScreen)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}
			
			
			}

		}


	);

	//FrontendUIDebugHelper::Log(TEXT("Remp Key Button Clicked"));
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyBindingButtonClicked()
{
	//FrontendUIDebugHelper::Log(TEXT("Reset Key Button Clicked"));
}
