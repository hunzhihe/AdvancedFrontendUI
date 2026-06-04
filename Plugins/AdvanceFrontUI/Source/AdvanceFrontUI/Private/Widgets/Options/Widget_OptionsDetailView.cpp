// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/Widget_OptionsDetailView.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "Widgets/Options/ListDataObject_Base.h"

void UWidget_OptionsDetailView::UpdateDetailView(const UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}

	TextBlock_Title->SetText(InDataObject->GetDataDisplayName());
	if (!InDataObject->GetSoftDiscriptionImage().IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDiscriptionImage());
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	CommonRichTextBlock_Description->SetText(InDataObject->GetDescriptionRichText());

	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class:<Bold>%s</>"),
		*InDataObject->GetClass()->GetName(), *InEntryWidgetClassName
	);

	CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetails));

	CommonRichTextBlock_DisableReason->SetText(InDataObject->GetDisabledRichText());
}

void UWidget_OptionsDetailView::ClearDetailView()
{

	TextBlock_Title->SetText(FText::GetEmpty());

	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);

	CommonRichTextBlock_Description->SetText(FText::GetEmpty());

	CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());

	CommonRichTextBlock_DisableReason->SetText(FText::GetEmpty());
}

void UWidget_OptionsDetailView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ClearDetailView();

}
