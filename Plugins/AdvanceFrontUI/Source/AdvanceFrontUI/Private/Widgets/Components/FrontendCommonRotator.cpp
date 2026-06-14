// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonRotator.h"
#include "CommonTextBlock.h"

void UFrontendCommonRotator::SetSelectedOptionByText(const FText& OptionText)
{
	// 在 TextLabels 数组中查找匹配的文本
   const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[OptionText](const FText& Text)
		{
			return Text.EqualTo(OptionText);
		}
	);

	// 找到则选中对应索引，未找到则直接显示文本
   if (FoundIndex != INDEX_NONE)
   {
	   SetSelectedItem(FoundIndex);
   }
   else
   {
	   MyText->SetText(OptionText);
   }

}
