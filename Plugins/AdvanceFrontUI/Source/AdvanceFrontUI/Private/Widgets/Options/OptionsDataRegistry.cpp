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
#include "Widgets/Options/DataObjects/ListDataObject_Stringbool.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"


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
			OverallVolum->SetOutputValueRange(TRange<float>(0.f, 10.f));
			OverallVolum->SetSliderStepSize(0.01f);
			OverallVolum->SetDefaultValueFromString(LexToString(1.f));
			OverallVolum->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolum->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			OverallVolum->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentOverallVolume));
			OverallVolum->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentOverallVolume));
			OverallVolum->SetShouldApplySettingsImmediately(true);


			//Set Data Dynamic getter and setter for the data object


			VolumeCategoryCollection->AddChildListData(OverallVolum);

		}

		//Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("This is description for Music Volume")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 10.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentMusicVolume));
			MusicVolume->SetShouldApplySettingsImmediately(true);


			//Set Data Dynamic getter and setter for the data object

			VolumeCategoryCollection->AddChildListData(MusicVolume);

		}

		//Sound FX Volume

		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(FText::FromString(TEXT("Sound FX Volume")));
			SoundFXVolume->SetDescriptionRichText(FText::FromString(TEXT("This is description for Sound FX Volume")));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 10.f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentSoundFXVolume));
			SoundFXVolume->SetShouldApplySettingsImmediately(true);


			//Set Data Dynamic getter and setter for the data object


			VolumeCategoryCollection->AddChildListData(SoundFXVolume);
		}




	



	}


	//Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Sound")));
		
		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		//Allow Background Audio
		{
			UListDataObject_Stringbool* AllowBackgroundAudio =  NewObject<UListDataObject_Stringbool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("Allow Background Audio")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enable")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		//User HDR Audio
		{
			UListDataObject_Stringbool* UserHDRAudio = NewObject<UListDataObject_Stringbool>();
			UserHDRAudio->SetDataID(FName("UserHDRAudio"));
			UserHDRAudio->SetDataDisplayName(FText::FromString(TEXT("User HDR Audio")));
			UserHDRAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enable")));
			UserHDRAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UserHDRAudio->SetFalseAsDefaultValue();
			UserHDRAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentUserHDRAudio));
			UserHDRAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentUserHDRAudio));
			UserHDRAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(UserHDRAudio);

		}

		
	}
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}



void UOptionsDataRegistry::InitVideoCollectionYab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("Video")));


	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Display")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptinsDataEditConditionDescriptor PackagedBulldOnlyCondition;
		PackagedBulldOnlyCondition.SetEditConditionFunc(
			[]()->bool {
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;

				return !bIsInEditor;
			}
		);

		PackagedBulldOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjust in a packaged build.</>"));

		//Window Mode
		{

			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("Window Mode")));
			WindowMode->SetDescriptionRichText(FText::FromString(TEXT("This is description for Window Mode")));
			WindowMode->AddEnumOptions(EWindowMode::Fullscreen, FText::FromString(TEXT("FullScreen Mode")));
			WindowMode->AddEnumOptions(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Window")));
			WindowMode->AddEnumOptions(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);

			WindowMode->AddEditCondition(PackagedBulldOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		//Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("Screen Resolution")));
			ScreenResolution->SetDescriptionRichText(FText::FromString(TEXT("This is description for Screen Resolution")));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);



			ScreenResolution->AddEditCondition(PackagedBulldOnlyCondition);
			ScreenResolution->AddEditDependencyData(CreatedWindowMode);


			FOptinsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool {

					const bool bIsBoderlessWindow =  CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == 
						EWindowMode::WindowedFullscreen;

					return !bIsBoderlessWindow;
				}
			
			);

			WindowModeEditCondition.SetDisabledRichReason(TEXT("\n\n<Disabled> Screen Resolution is not adjustable when the 'window mode' is set to Borderless Window.the value must match with the maximum allowed resolution. </>"));
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);

		}



	}

	//Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Graphics")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		//Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("Brightness")));
			DisplayGamma->SetDescriptionRichText(FText::FromString(TEXT("This is description for Display Gamma")));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f));
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDefaultValueFromString(LexToString(1.f));
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));


			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInteger* CreatedOverallQuality = nullptr;

		//Overall Quality
		{
			UListDataObject_StringInteger* OverallQuality =  NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString(TEXT("Overall Quality")));
			OverallQuality->SetDescriptionRichText(FText::FromString(TEXT("This is description for OverallQuality")));
			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);
			

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		//Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("3D Resolution")));
			ResolutionScale->SetDescriptionRichText(FText::FromString(TEXT("This is description for ResolutionScale")));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);


			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}


	}



	RegisteredOptionsTabCollections.Add(VideoTabCollection);
	
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("Control")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);

}
