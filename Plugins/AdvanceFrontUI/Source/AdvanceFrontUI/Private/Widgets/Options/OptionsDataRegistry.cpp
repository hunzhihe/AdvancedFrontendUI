// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"
#include "FrontendSettings/FrontendDeveloperSettings.h"
#include "FrontendUIFunctionLibrary.h"
#include "FrontendGameplayTags.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"


#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
 MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendUIGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionYab();
	InitControlCollectionTab();

}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr =  RegisteredOptionsTabCollections.FindByPredicate([InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		});

	checkf(FoundTabCollectionPtr,TEXT("No matching tab collection found for ID: %s"), *InSelectedTabID.ToString());
	
		UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

		TArray<UListDataObject_Base*> AllChildListItems;

		for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
		{
			if (!ChildListData)
			{
				continue;
			}

			AllChildListItems.Add(ChildListData);

			if (ChildListData->HasAnyChildListData())
			{
				FindChildListDataRecursively(ChildListData, AllChildListItems);
			}
		}

		return AllChildListItems;
	
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParantData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParantData || !InParantData->HasAnyChildListData())
	{
		return;
	}
	for (UListDataObject_Base* SubChilListData : InParantData->GetAllChildListData())
	{
		if (!SubChilListData) {

			continue;
		}

		OutFoundChildListData.Add(SubChilListData);

		if (SubChilListData->HasAnyChildListData()) 
		{
			FindChildListDataRecursively(SubChilListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("Gameplay")));

	
	//TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =
	//	MakeShared<FOptionsDataInteractionHelper>(
	//		GET_FUNCTION_NAME_STRING_CHECKED(UFrontendUIGameUserSettings, GetCurrentGameDifficulty));
	

	//Game Difficulty
	{
		UListDataObject_String* GameDifficultyOption = NewObject<UListDataObject_String>();
		GameDifficultyOption->SetDataID(FName("GameDifficultyOption"));
		GameDifficultyOption->SetDataDisplayName(FText::FromString(TEXT("Game Difficulty")));
		GameDifficultyOption->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));

		GameDifficultyOption->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficultyOption->AddDynamicOption(TEXT("Medium"), FText::FromString(TEXT("Medium")));
		GameDifficultyOption->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficultyOption->SetDefaultValueFromString(TEXT("Medium"));

		GameDifficultyOption->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficultyOption->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficultyOption->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficultyOption);
	}
	//Test Item
	{
		UListDataObject_String* TestItemOption = NewObject<UListDataObject_String>();
		TestItemOption->SetDataID(FName("TestItemOption"));
		TestItemOption->SetDataDisplayName(FText::FromString(TEXT("Test Image Item")));
		TestItemOption->SetSoftDiscriptionImage(UFrontendUIFunctionLibrary::GetOptionsSoftImage(FrontendGameplayTags::Frontend_Image_TestIamge));
		TestItemOption->SetDescriptionRichText(FText::FromString(TEXT("The Image to display can be specified in the settings,it can be anything the Developer assigned in there")));


		GameplayTabCollection->AddChildListData(TestItemOption);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	

	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume")));


		AudioTabCollection->AddChildListData(VolumeCategoryCollection);


		//Overall Volum
		{
			UListDataObject_Scalar* OverallVolum = NewObject<UListDataObject_Scalar>();
			OverallVolum->SetDataID(FName("OverallVolum"));
			OverallVolum->SetDataDisplayName(FText::FromString(TEXT("Overall Volum")));
			OverallVolum->SetDescriptionRichText(FText::FromString(TEXT("This is description for Overall Volum")));
			OverallVolum->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolum->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolum->SetSliderStepSize(0.01f);
			OverallVolum->SetDefaultValueFromString(LexToString(1.f));
			OverallVolum->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolum->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			//Set Data Dynamic getter and setter for the data object


			VolumeCategoryCollection->AddChildListData(OverallVolum);


		}




	}


	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}



void UOptionsDataRegistry::InitVideoCollectionYab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
	
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);

}
