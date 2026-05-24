// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"


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

    if (CommonTextBlock_ButtonText)
    {
        CommonTextBlock_ButtonText->SetStyle(GetCurrentTextStyleClass());
    }
    

}