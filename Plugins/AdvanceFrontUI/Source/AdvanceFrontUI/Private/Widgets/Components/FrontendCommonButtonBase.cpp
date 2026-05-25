// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystem/FrontendUISubsystem.h"


void UFrontendCommonButtonBase::SetButtonText(FText NewText)
{
    if(CommonTextBlock_ButtonText && !NewText.IsEmpty())
    {
        CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? NewText.ToUpper() : NewText);
    }
}

void UFrontendCommonButtonBase::NativePreConstruct()
{
    Super::NativePreConstruct();

    SetButtonText(ButtonDisplayText);
}
void UFrontendCommonButtonBase::NativeOnCurrentTextStyleChanged()
{
    Super::NativeOnCurrentTextStyleChanged();

    if (CommonTextBlock_ButtonText && GetCurrentTextStyleClass())
    {
        CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
    }
}
void UFrontendCommonButtonBase::NativeOnHovered()
{
    Super::NativeOnHovered();
    if (!ButtonDiscriptionText.IsEmpty())
    {
        UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this);
        if (FrontendUISubsystem)        
        {
            FrontendUISubsystem->OnButtonDescriptionTextUpdated.Broadcast(this, ButtonDiscriptionText);
        }
    }      
}

void UFrontendCommonButtonBase::NativeOnUnhovered()
{
    Super::NativeOnUnhovered();
    UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this);
    if (FrontendUISubsystem)    {
        FrontendUISubsystem->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
    }
}
