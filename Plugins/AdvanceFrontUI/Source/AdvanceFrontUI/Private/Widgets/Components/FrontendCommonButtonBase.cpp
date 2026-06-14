// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonTextBlock.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "Widgets/Components/FrontendUIButtonInternalBase.h"
#include "Framework/Application/SlateApplication.h"
#include "Blueprint/WidgetTree.h"
#include "CommonButtonBase.h"
#include "CommonLazyImage.h"




void UFrontendCommonButtonBase::SetButtonText(FText NewText)
{
    if(CommonTextBlock_ButtonText && !NewText.IsEmpty())
    {
        CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? NewText.ToUpper() : NewText);
    }
}

FText UFrontendCommonButtonBase::GetButtonDisplayText() const
{
    if (CommonTextBlock_ButtonText)
    {
        CommonTextBlock_ButtonText->GetText();
    }

    return FText();
}

void UFrontendCommonButtonBase::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
    if (CommonLazyImage_ButtonImage)
    {
        CommonLazyImage_ButtonImage->SetBrush(InBrush);
    }
}



void UFrontendCommonButtonBase::NativePreConstruct()
{
    Super::NativePreConstruct();

    SetButtonText(ButtonDisplayText);
}
UCommonButtonInternalBase* UFrontendCommonButtonBase::ConstructInternalButton()
{
    // 通过 WidgetTree 创建我们的自定义内部按钮实例
    CachedInternalButton = WidgetTree->ConstructWidget<UFrontendUIButtonInternalBase>(
        UFrontendUIButtonInternalBase::StaticClass(),
        FName(TEXT("InternalRootButtonBase"))
    );
    return CachedInternalButton;
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
    // 悬停时通过子系统广播按钮描述文本
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
    // 取消悬停时清除描述文本
    UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this);
    if (FrontendUISubsystem)    {
        FrontendUISubsystem->OnButtonDescriptionTextUpdated.Broadcast(this, FText::GetEmpty());
    }
}

//FReply UFrontendCommonButtonBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
//{
    //Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

    // 通过 WidgetTree 查找名为 "InternalRootButtonBase" 的控件（这是 CommonButton 内部约定的名称）
    //UWidget* RootWidget = WidgetTree->RootWidget;
    //UButton* InternalButton = Cast<UButton>(RootWidget);
    //if (!InternalButton)
    //    return FReply::Unhandled();

    //TSharedPtr<SWidget> SlateWidget = InternalButton->GetCachedWidget();
    //if (SlateWidget.IsValid() && SlateWidget->SupportsKeyboardFocus())
    //{
    //    FWidgetPath FocusPath = FSlateApplication::Get().GeneratePathToWidgetChecked(SlateWidget.ToSharedRef());
    //    FSlateApplication::Get().SetKeyboardFocus(FocusPath, EFocusCause::Navigation);
      // return FReply::Handled();
    //}
    //return FReply::Unhandled();
//}

