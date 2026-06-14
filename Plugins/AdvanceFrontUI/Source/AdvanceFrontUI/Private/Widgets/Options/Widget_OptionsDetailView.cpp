// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/Widget_OptionsDetailView.h"
#include "CommonTextBlock.h"
#include "CommonLazyImage.h"
#include "CommonRichTextBlock.h"
#include "Widgets/Options/ListDataObject_Base.h"

void UWidget_OptionsDetailView::UpdateDetailView(UListDataObject_Base* InDataObject, const FString& InEntryWidgetClassName)
{
	if (!InDataObject)
	{
		return;
	}

	// 设置标题
	TextBlock_Title->SetText(InDataObject->GetDataDisplayName());

	// 设置描述图片（有则显示，无则隐藏）
	if (!InDataObject->GetSoftDiscriptionImage().IsNull())
	{
		CommonLazyImage_DescriptionImage->SetBrushFromLazyTexture(InDataObject->GetSoftDiscriptionImage());
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 设置富文本描述
	CommonRichTextBlock_Description->SetText(InDataObject->GetDescriptionRichText());

	// 设置动态详情（数据对象类名 + 条目控件类名）
	const FString DynamicDetails = FString::Printf(
		TEXT("Data Object Class: <Bold>%s</>\n\nEntry Widget Class:<Bold>%s</>"),
		*InDataObject->GetClass()->GetName(), *InEntryWidgetClassName
	);

	CommonRichTextBlock_DynamicDetails->SetText(FText::FromString(DynamicDetails));

	// 设置禁用原因（仅在不可编辑时显示）
	CommonRichTextBlock_DisableReason->SetText(InDataObject->
		IsDataCurrentlyEditable()?FText::GetEmpty() : InDataObject->GetDisabledRichText()

	);
}

void UWidget_OptionsDetailView::ClearDetailView()
{
	// 清空所有详情字段
	TextBlock_Title->SetText(FText::GetEmpty());

	CommonLazyImage_DescriptionImage->SetVisibility(ESlateVisibility::Collapsed);

	CommonRichTextBlock_Description->SetText(FText::GetEmpty());

	CommonRichTextBlock_DynamicDetails->SetText(FText::GetEmpty());

	CommonRichTextBlock_DisableReason->SetText(FText::GetEmpty());
}

void UWidget_OptionsDetailView::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 初始时清空详情视图
	ClearDetailView();

}
