// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/OptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

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
		return FoundTabCollection->GetAllChildListData();
	
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
		GameDifficultyOption->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficultyOption->AddDynamicOption(TEXT("Medium"), FText::FromString(TEXT("Medium")));
		GameDifficultyOption->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));

		GameDifficultyOption->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficultyOption->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficultyOption->SetShouldApplySettingsImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficultyOption);
	}
	//Test Item
	{
		UListDataObject_String* TestItemOption = NewObject<UListDataObject_String>();
		TestItemOption->SetDataID(FName("TestItemOption"));
		TestItemOption->SetDataDisplayName(FText::FromString(TEXT("Test Item")));

		GameplayTabCollection->AddChildListData(TestItemOption);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	

	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("Audio")));

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
