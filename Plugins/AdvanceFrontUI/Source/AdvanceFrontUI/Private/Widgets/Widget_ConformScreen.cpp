// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_ConformScreen.h"
#include "CommonTextBlock.h"
#include "Components/DynamicEntryBox.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "ICommonInputModule.h"


UConfirmScreenInfoObject* UConfirmScreenInfoObject::CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage)
{
	UConfirmScreenInfoObject* InfoObject =  NewObject<UConfirmScreenInfoObject>();
    InfoObject->ScreenTitle = InScreenTitle;
    InfoObject->ScreenMessage = InScreenMessage;

    FConformScreenButtonInfo OKButtonInfo;
    OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Closed;
    OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("OK"));

    InfoObject->AvailableDcreenButtons.Add(OKButtonInfo);

    return InfoObject;
}
UConfirmScreenInfoObject *UConfirmScreenInfoObject::CreateYesNoScreen(const FText &InScreenTitle, const FText &InScreenMessage)
{
    UConfirmScreenInfoObject* InfoObject =  NewObject<UConfirmScreenInfoObject>();
    InfoObject->ScreenTitle = InScreenTitle;
    InfoObject->ScreenMessage = InScreenMessage;

    FConformScreenButtonInfo YesButtonInfo;
    YesButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    YesButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Yes"));

    FConformScreenButtonInfo NoButtonInfo;
    NoButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    NoButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("No"));

    InfoObject->AvailableDcreenButtons.Add(YesButtonInfo);
    InfoObject->AvailableDcreenButtons.Add(NoButtonInfo);

    return InfoObject;
}

UConfirmScreenInfoObject *UConfirmScreenInfoObject::CreateOKCancelScreen(const FText &InScreenTitle, const FText &InScreenMessage)
{
    UConfirmScreenInfoObject* InfoObject =  NewObject<UConfirmScreenInfoObject>();
    InfoObject->ScreenTitle = InScreenTitle;
    InfoObject->ScreenMessage = InScreenMessage;

    FConformScreenButtonInfo OKButtonInfo;
    OKButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Confirmed;
    OKButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("OK"));

    FConformScreenButtonInfo CancelButtonInfo;
    CancelButtonInfo.ConfirmScreenButtonType = EConfirmScreenButtonType::Cancelled;
    CancelButtonInfo.ButtonTextToDisplay = FText::FromString(TEXT("Cancel"));

    InfoObject->AvailableDcreenButtons.Add(OKButtonInfo);
    InfoObject->AvailableDcreenButtons.Add(CancelButtonInfo);

    return InfoObject;
}

void UWidget_ConformScreen::InitConformScreen(UConfirmScreenInfoObject *InConfirmScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> InButtonClickedCallback)
{
    if (CommonTextBlock_ConformScreenTitle && CommonTextBlock_ConformScreenMessage && InConfirmScreenInfoObject)
    {
        CommonTextBlock_ConformScreenTitle->SetText(InConfirmScreenInfoObject->ScreenTitle);
        CommonTextBlock_ConformScreenMessage->SetText(InConfirmScreenInfoObject->ScreenMessage);

        if (DynamicEntryBox_ConformScreenButton->GetNumEntries() != 0)
        {
            DynamicEntryBox_ConformScreenButton->Reset<UFrontendCommonButtonBase>(
            [](UFrontendCommonButtonBase& ExistingButton)
            {
                ExistingButton.OnClicked().Clear();
            }
            );
        }

        if (InConfirmScreenInfoObject->AvailableDcreenButtons.Num() > 0)
        {

            for (const FConformScreenButtonInfo& AvailableButtonInfo : InConfirmScreenInfoObject->AvailableDcreenButtons)
            {

                FDataTableRowHandle InputActionRowHandle;

                switch (AvailableButtonInfo.ConfirmScreenButtonType)
                {
                case EConfirmScreenButtonType::Confirmed:
                    InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultClickAction();
                    break;
                case EConfirmScreenButtonType::Cancelled:
                    InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
                    break;
                case EConfirmScreenButtonType::Closed:
                    InputActionRowHandle = ICommonInputModule::GetSettings().GetDefaultBackAction();
                     break;
                default:
                    break;
                    
                }

                UFrontendCommonButtonBase* Button = DynamicEntryBox_ConformScreenButton->CreateEntry<UFrontendCommonButtonBase>();
                if (Button)
                {
                    Button->SetButtonText(AvailableButtonInfo.ButtonTextToDisplay);
                    Button->SetTriggeringInputAction(InputActionRowHandle);
                    Button->OnClicked().AddLambda
                    ([InButtonClickedCallback, AvailableButtonInfo, this]()
                    {
                        InButtonClickedCallback(AvailableButtonInfo.ConfirmScreenButtonType);
                        DeactivateWidget();

                    });
                }
            }
        
            if (DynamicEntryBox_ConformScreenButton->GetNumEntries() > 0)
            {
                DynamicEntryBox_ConformScreenButton->GetAllEntries().Last()->SetFocus();
            }
        }   
    
    }
}