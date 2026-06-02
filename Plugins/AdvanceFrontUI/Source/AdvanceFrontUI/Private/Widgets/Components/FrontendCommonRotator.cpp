// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendCommonRotator.h"
#include "CommonTextBlock.h"

void UFrontendCommonRotator::SetSelectedOptionByText(const FText& OptionText)
{
   const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[OptionText](const FText& Text)
		{
			return Text.EqualTo(OptionText);
		}
	);

   if (FoundIndex != INDEX_NONE)
   {
	   SetSelectedItem(FoundIndex);
   }
   else
   {
	   MyText->SetText(OptionText);
   }

}
