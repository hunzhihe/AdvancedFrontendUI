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
#include "Internationalization/StringTableRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Subsystem/FrontendUISubsystem.h"


#include "FrontendUIDebugHelper.h"



#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
 MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendUIGameUserSettings, SetterOrGetterFuncName))
#define GET_DESCRIPTION(InKey)LOCTABLE("/Game/Blueprints/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription",  InKey)
#define GET_Zh_DESCRIPTION(InKey)LOCTABLE("/Game/Blueprints/UI/StringTable/ST_ZH_OptionScreenDescription.ST_ZH_OptionScreenDescription", InKey)


void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionYab();
	InitControlCollectionTab(InOwningLocalPlayer);

	// 初始化完成后统一刷新所有本地化文本（显示名称、描述等）
	RefreshAllLocalizedText();

	// 获取 World（通过 LocalPlayer 的 ViewportClient）
	//UWorld* World = InOwningLocalPlayer->GetWorld();
	//if (World)
	//{
	//	CachedWorld = World;
	//}

	//if (UFrontendUISubsystem* UISubsystem = UFrontendUISubsystem::Get(CachedWorld->GetWorld()))
	//{
	//	FrontendUIDebugHelper::Log(TEXT("FrontendUISubsystem is ready"));

	//	UISubsystem->OnRegistryNewOptionsData.BindUObject(this, &ThisClass::NewLaughageTableRegistry);
	//};


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

//void UOptionsDataRegistry::NewLaughageTableRegistry(ULocalPlayer* InOwningLocalPlayer)
//{
//	InitGameplayCollectionTab();
//	InitAudioCollectionTab();
//	InitVideoCollectionYab();
//	InitControlCollectionTab(InOwningLocalPlayer);
//}

void UOptionsDataRegistry::RefreshAllLocalizedText()
{
	for (UListDataObject_Collection* TabCollection : RegisteredOptionsTabCollections)
	{
		if (!TabCollection)
		{
			continue;
		}

		// 刷新选项卡集合自身的本地化文本
		TabCollection->RefreshLocalizedText();

		// 递归刷新所有子级
		RefreshChildListDataRecursively(TabCollection);
	}
}

void UOptionsDataRegistry::RefreshChildListDataRecursively(UListDataObject_Base* InParantData)
{
	if (!InParantData || !InParantData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* ChildData : InParantData->GetAllChildListData())
	{
		if (!ChildData)
		{
			continue;
		}

		// 刷新该数据对象的本地化文本
		ChildData->RefreshLocalizedText();

		// 递归处理其子级
		if (ChildData->HasAnyChildListData())
		{
			RefreshChildListDataRecursively(ChildData);
		}
	}
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
	GameplayTabCollection->SetDisplayNameLocalizationKey(TEXT("DN_Gameplay"));

	
	//TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =
	//	MakeShared<FOptionsDataInteractionHelper>(
	//		GET_FUNCTION_NAME_STRING_CHECKED(UFrontendUIGameUserSettings, GetCurrentGameDifficulty));
	

	//Game Difficulty
	{
		UListDataObject_String* GameDifficultyOption = NewObject<UListDataObject_String>();
		GameDifficultyOption->SetDataID(FName("GameDifficultyOption"));
		GameDifficultyOption->SetDisplayNameLocalizationKey(TEXT("DN_GameDifficulty"));

		//描述文本通过本地化键在语言切换时自动刷新
		GameDifficultyOption->SetDescriptionLocalizationKey(TEXT("GameDifficulty"));
		// RefreshLocalizedText 由 InitOptionsDataRegistry 末尾统一调用
		//GameDifficultyOption->SetDescriptionRichText(FText::FromString(TEXT("Adjusts the difficulty of the game experience.\n\n<Bold>Easy:</> Focuses on the story experience. Provides the most relaxing combat.\n\n<Bold>Normal:</> Offers slightly harder combat experience\n\n<Bold>Hard:</> Offers a much more challenging combat experience\n\n<Bold>Vert Hard:</> Provides the most challenging combat experience. Not recommended for first play through.")));
		//GameDifficultyOption->SetDescriptionRichText(UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::ZH_Ch, "WindowModeDescKey"));

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
		TestItemOption->SetDisplayNameLocalizationKey(TEXT("DN_TestImageItem"));
		TestItemOption->SetSoftDiscriptionImage(UFrontendUIFunctionLibrary::GetOptionsSoftImage(FrontendGameplayTags::Frontend_Image_TestIamge));
		TestItemOption->SetDescriptionRichText(FText::FromString(TEXT("The Image to display can be specified in the settings,it can be anything the Developer assigned in there")));

		GameplayTabCollection->AddChildListData(TestItemOption);
	}
    

	//Language
	{
		//const FText WindowModeDesc =  LOCTABLE("/Game/Blueprints/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription","WindowModeDescKey");

		UListDataObject_String* Language = NewObject<UListDataObject_String>();
		Language->SetDataID(FName("Language"));
		Language->SetDisplayNameLocalizationKey(TEXT("DN_Language"));

		//if (UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetCurrentLanguage() == ELaughageChanged::English)
		//{
		//	Language->AddDynamicOption(TEXT("English"), UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::English, "English"));
		//	Language->AddDynamicOption(TEXT("ZH_Ch"), UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::English, "ZH_Ch"));

		//}
		//else if (UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetCurrentLanguage() == ELaughageChanged::ZH_Ch)
		//{
		//	Language->AddDynamicOption(TEXT("English"), UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::ZH_Ch, "English"));
		//	Language->AddDynamicOption(TEXT("ZH_Ch"), UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::ZH_Ch, "ZH_Ch"));
		//}
		//Language->SetDescriptionRichText(GET_DESCRIPTION("LanguageDescKey"));
		Language->AddDynamicOption(TEXT("English"), UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::English, "English"));
		Language->AddDynamicOption(TEXT("ZH_Ch"), UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::English, "ZH_Ch"));
		Language->SetDefaultValueFromString(TEXT("English"));
		Language->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentLanguage));
		Language->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentLanguage));
		Language->SetShouldApplySettingsImmediately(true);


		GameplayTabCollection->AddChildListData(Language);
	}



	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	

	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDisplayNameLocalizationKey(TEXT("DN_Audio"));

	//Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDisplayNameLocalizationKey(TEXT("DN_Volume"));


		AudioTabCollection->AddChildListData(VolumeCategoryCollection);


		//Overall Volum
		{
			UListDataObject_Scalar* OverallVolum = NewObject<UListDataObject_Scalar>();
			OverallVolum->SetDataID(FName("OverallVolum"));
			OverallVolum->SetDisplayNameLocalizationKey(TEXT("DN_OverallVolum"));
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
			MusicVolume->SetDisplayNameLocalizationKey(TEXT("DN_MusicVolume"));
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
			SoundFXVolume->SetDisplayNameLocalizationKey(TEXT("DN_SoundFXVolume"));
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
		SoundCategoryCollection->SetDisplayNameLocalizationKey(TEXT("DN_Sound"));
		
		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		//Allow Background Audio
		{
			UListDataObject_Stringbool* AllowBackgroundAudio =  NewObject<UListDataObject_Stringbool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDisplayNameLocalizationKey(TEXT("DN_AllowBackgroundAudio"));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enable")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			AllowBackgroundAudio->SetTrueLocalizationKey(TEXT("DN_Enable"));
			AllowBackgroundAudio->SetFalseLocalizationKey(TEXT("DN_Disabled"));
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
			UserHDRAudio->SetDisplayNameLocalizationKey(TEXT("DN_UserHDRAudio"));
			UserHDRAudio->OverrideTrueDisplayText(FText::FromString(TEXT("Enable")));
			UserHDRAudio->OverrideFalseDisplayText(FText::FromString(TEXT("Disabled")));
			UserHDRAudio->SetTrueLocalizationKey(TEXT("DN_Enable"));
			UserHDRAudio->SetFalseLocalizationKey(TEXT("DN_Disabled"));
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
	VideoTabCollection->SetDisplayNameLocalizationKey(TEXT("DN_Video"));


	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDisplayNameLocalizationKey(TEXT("DN_Display"));

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
			//const FText WindowModeDesc =  LOCTABLE("/Game/Blueprints/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription","WindowModeDescKey");
			
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDisplayNameLocalizationKey(TEXT("DN_WindowMode"));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
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
			//const FText ScreenResolutionDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "ScreenResolutionsDescKey");
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDisplayNameLocalizationKey(TEXT("DN_ScreenResolution"));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionsDescKey"));
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
		GraphicsCategoryCollection->SetDisplayNameLocalizationKey(TEXT("DN_Graphics"));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		//Display Gamma
		{
			//const FText DisplayGammaDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "DisplayGammaDescKey");
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDisplayNameLocalizationKey(TEXT("DN_Brightness"));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
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

		// 为所有图形质量选项设置统一的本地化键（Low/Normal/High/Epic/Cinematic）
		auto SetQualityLocalizationKeys = [](UListDataObject_StringInteger* QualityOption)
		{
			QualityOption->SetLocalizationKeyForIntegerValue(0, TEXT("DN_Quality_Low"));
			QualityOption->SetLocalizationKeyForIntegerValue(1, TEXT("DN_Quality_Normal"));
			QualityOption->SetLocalizationKeyForIntegerValue(2, TEXT("DN_Quality_High"));
			QualityOption->SetLocalizationKeyForIntegerValue(3, TEXT("DN_Quality_Epic"));
			QualityOption->SetLocalizationKeyForIntegerValue(4, TEXT("DN_Quality_Cinematic"));
		};

		//Overall Quality
		{
			//const FText OverallQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "OverallQualityDescKey");
			UListDataObject_StringInteger* OverallQuality =  NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDisplayNameLocalizationKey(TEXT("DN_OverallQuality"));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);

			SetQualityLocalizationKeys(OverallQuality);

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		//Resolution Scale
		{
			//const FText ResolutionScaleDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "ResolutionScaleDescKey");
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDisplayNameLocalizationKey(TEXT("DN_3DResolution"));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);


			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		//Global Illumination Quality
		{
			//const FText GlobalIlluminationQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "GlobalIlluminationQualityDescKey");
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDisplayNameLocalizationKey(TEXT("DN_GlobalIllumination"));
			GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(GlobalIlluminationQuality);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);

		}



		//Shadow Quality
		{
			//const FText ShadowQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "ShadowQualityDescKey");
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDisplayNameLocalizationKey(TEXT("DN_ShadowQuality"));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(ShadowQuality);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);

		}

		//AntiAliasing Quality
		{
			//const FText AntiAliasingDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "AntiAliasingDescKey");
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDisplayNameLocalizationKey(TEXT("DN_AntiAliasing"));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingDescKey"));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(AntiAliasingQuality);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		
		}

		//View Distance Quality
		{
			//const FText ViewDistanceDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "ViewDistanceDescKey");
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDisplayNameLocalizationKey(TEXT("DN_ViewDistance"));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(ViewDistanceQuality);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}


		//Texture Quality
		{

			//const FText TextureQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "TextureQualityDescKey");
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDisplayNameLocalizationKey(TEXT("DN_TextureQuality"));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(TextureQuality);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);

		}

		//Visual Effects Quality
		{
			//const FText VisualEffectQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "VisualEffectQualityDescKey");
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectsQuality"));
			VisualEffectQuality->SetDisplayNameLocalizationKey(TEXT("DN_VisualEffects"));
			VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
			VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(VisualEffectQuality);

			VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);

		}

		//Reflection Quality

		{
			//const FText ReflectionQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "ReflectionQualityDescKey");
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDisplayNameLocalizationKey(TEXT("DN_ReflectionQuality"));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(ReflectionQuality);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);



		}

		//Post Processing Quality

		{
			//const FText PostProcessingQualityDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "PostProcessingQualityDescKey");
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDisplayNameLocalizationKey(TEXT("DN_PostProcessing"));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Normal")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("Epic")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Cinematic")));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);
			SetQualityLocalizationKeys(PostProcessingQuality);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}


	}

	//Advanced Graphics Category
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDisplayNameLocalizationKey(TEXT("DN_AdvancedGraphics"));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		//Vertical Sync
		{
			//const FText VerticalSyncDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "VerticalSyncDescKey");
			UListDataObject_Stringbool* VerticalSync = NewObject<UListDataObject_Stringbool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDisplayNameLocalizationKey(TEXT("DN_VSync"));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetTrueLocalizationKey(TEXT("DN_ON"));
			VerticalSync->SetFalseLocalizationKey(TEXT("DN_OFF"));
			VerticalSync->SetShouldApplySettingsImmediately(true);

			FOptinsDataEditConditionDescriptor FullScreenOnlyCondition;
			FullScreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool {

					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;

				}
			

			);

			FullScreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled> This feature only works if the window Mode is set to 'FullScreen'.</>"));
			FullScreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));



			VerticalSync->AddEditCondition(FullScreenOnlyCondition);
			
			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		//Frame Rate Limit
		{

			//const FText FrameRateLimitDesc = LOCTABLE(" / Game / Blueprints / UI / StringTable / ST_OptionsScreenDescription.ST_OptionsScreenDescription", "FrameRateLimitDescKey");
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDisplayNameLocalizationKey(TEXT("DN_FrameRateLimit"));
			//FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("No FPS Limit")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediately(true);


			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}


	RegisteredOptionsTabCollections.Add(VideoTabCollection);
	
}

void UOptionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwningLocalPlayer)
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDisplayNameLocalizationKey(TEXT("DN_Control"));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();

	check(EIUserSettings);

	//Keyboard Mouse Category
	{
		UListDataObject_Collection* KeyboardMouseCategory = NewObject<UListDataObject_Collection>();
		KeyboardMouseCategory->SetDataID(FName("KeyboardMouseCategory"));
		KeyboardMouseCategory->SetDisplayNameLocalizationKey(TEXT("DN_KeyboardMouse"));


		ControlTabCollection->AddChildListData(KeyboardMouseCategory);
	
		//Keyboard Mouse Input
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S;
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;

			//测试
			//FPlayerMappableKeyQueryOptions GamepadOnly;
			//GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Right;
			//GamepadOnly.bMatchBasicKeyTypes = true;

			for (const TPair<FString, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for(const FPlayerKeyMapping& KeyMappinging : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMappinging, KeyboardMouseOnly))
						{
							//FrontendUIDebugHelper::Log(TEXT("Mapping ID:") + KeyMappinging.GetMappingName().ToString() +
							//TEXT("Display Name:") + KeyMappinging.GetDisplayName().ToString() +
							//TEXT("Bound Key:") + KeyMappinging.GetCurrentKey().GetDisplayName().ToString()
						 //   );
							UListDataObject_KeyRemap* KeyRemapDataObject =  NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMappinging.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMappinging.GetDisplayName());
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMappinging);



							KeyboardMouseCategory->AddChildListData(KeyRemapDataObject);
						}


						
					}
				}
			}
		}


		


	}


	//GamePad Category
	{
		UListDataObject_Collection* GamePadCategory = NewObject<UListDataObject_Collection>();
		GamePadCategory->SetDataID(FName("GamePadCategory"));
		GamePadCategory->SetDisplayNameLocalizationKey(TEXT("DN_GamePad"));


		ControlTabCollection->AddChildListData(GamePadCategory);

		//GamePad Input
		{
			FPlayerMappableKeyQueryOptions GamePadOnly;
			GamePadOnly.KeyToMatch = EKeys::EKeys::Gamepad_FaceButton_Right;
			GamePadOnly.bMatchBasicKeyTypes = true;

			//测试
			//FPlayerMappableKeyQueryOptions GamepadOnly;
			//GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Right;
			//GamepadOnly.bMatchBasicKeyTypes = true;

			for (const TPair<FString, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllAvailableKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;

				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMappinging : MappingRowPair.Value.Mappings)
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMappinging, GamePadOnly))
						{
							//FrontendUIDebugHelper::Log(TEXT("Mapping ID:") + KeyMappinging.GetMappingName().ToString() +
							//TEXT("Display Name:") + KeyMappinging.GetDisplayName().ToString() +
							//TEXT("Bound Key:") + KeyMappinging.GetCurrentKey().GetDisplayName().ToString()
						 //   );
							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMappinging.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMappinging.GetDisplayName());
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMappinging);



							GamePadCategory->AddChildListData(KeyRemapDataObject);
						}



					}
				}
			}
		}





	}


	RegisteredOptionsTabCollections.Add(ControlTabCollection);

}
