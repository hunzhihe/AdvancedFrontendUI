// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/CreatCharacterScreenDataRegistry.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "PlayerSaveData.h"


void UCreatCharacterScreenDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
    // 创建运行时 PlayerSaveData 实例，所有数据对象将通过 FOptionsDataInteractionHelper 桥接到它
    CreatedPlayerSaveData = NewObject<UPlayerSaveData>();

    InitGameCharacterCollectionTab();

    // 初始化完成后统一刷新所有本地化文本（显示名称、描述等）
    RefreshAllLocalizedText();
}

TArray<UListDataObject_Base*> UCreatCharacterScreenDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
    UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredCharacterCreateTabCollections.FindByPredicate([InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
        {
            return AvailableTabCollection->GetDataID() == InSelectedTabID;
        });

    checkf(FoundTabCollectionPtr, TEXT("No matching tab collection found for ID: %s"), *InSelectedTabID.ToString());

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

void UCreatCharacterScreenDataRegistry::RefreshAllLocalizedText()
{
    for (UListDataObject_Collection* TabCollection : RegisteredCharacterCreateTabCollections)
    {
        if (!TabCollection)
        {
            continue;
        }

        TabCollection->RefreshLocalizedText();
        RefreshChildListDataRecursively(TabCollection);
    }
}

void UCreatCharacterScreenDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParantData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
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

void UCreatCharacterScreenDataRegistry::RefreshChildListDataRecursively(UListDataObject_Base* InParantData)
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

        ChildData->RefreshLocalizedText();

        if (ChildData->HasAnyChildListData())
        {
            RefreshChildListDataRecursively(ChildData);
        }
    }
}

void UCreatCharacterScreenDataRegistry::InitGameCharacterCollectionTab()
{
    // 辅助 lambda：创建绑定到 CreatedPlayerSaveData 的 FOptionsDataInteractionHelper
    auto MakeControl = [this](const FString& FuncName) -> TSharedPtr<FOptionsDataInteractionHelper>
    {
        return MakeShared<FOptionsDataInteractionHelper>(FuncName, CreatedPlayerSaveData);
    };

    // ========== 角色基本信息选项卡 ==========
    UListDataObject_Collection* CreateCharacterInfoTabCollection = NewObject<UListDataObject_Collection>();
    CreateCharacterInfoTabCollection->SetDataID(FName("CreateCharacterInfo"));
    CreateCharacterInfoTabCollection->SetDisplayNameLocalizationKey(TEXT("DN_CreateCharacterInfo"));

    // 角色名称
    {
        UListDataObject_String* CharacterName = NewObject<UListDataObject_String>();
        CharacterName->SetDataID(FName("CharacterNameOption"));
        CharacterName->SetDisplayNameLocalizationKey(TEXT("DN_CharacterName"));
        CharacterName->SetDescriptionLocalizationKey(TEXT("CharacterNameDesc"));

        CharacterName->AddDynamicOption(TEXT("DefaultName"), FText::FromString(TEXT("DefaultName")));
        CharacterName->SetDefaultValueFromString(TEXT("DefaultName"));

        CharacterName->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentCharacterName)));
        CharacterName->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentCharacterName)));
        CharacterName->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(CharacterName);
    }

    // 角色性别
    {
        UListDataObject_String* CharacterSex = NewObject<UListDataObject_String>();
        CharacterSex->SetDataID(FName("CharacterSexOption"));
        CharacterSex->SetDisplayNameLocalizationKey(TEXT("DN_CharacterSex"));
        CharacterSex->SetDescriptionLocalizationKey(TEXT("CharacterSexDesc"));

        CharacterSex->AddDynamicOption(TEXT("male"), FText::FromString(TEXT("male")));
        CharacterSex->AddDynamicOption(TEXT("female"), FText::FromString(TEXT("female")));
        CharacterSex->SetDefaultValueFromString(TEXT("male"));

        CharacterSex->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentCharacterSex)));
        CharacterSex->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentCharacterSex)));
        CharacterSex->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(CharacterSex);
    }

    // 出生地
    {
        UListDataObject_String* Birthplace = NewObject<UListDataObject_String>();
        Birthplace->SetDataID(FName("BirthplaceOption"));
        Birthplace->SetDisplayNameLocalizationKey(TEXT("DN_Birthplace"));
        Birthplace->SetDescriptionLocalizationKey(TEXT("BirthplaceDesc"));

        Birthplace->AddDynamicOption(TEXT("NorthernKingdom"), FText::FromString(TEXT("Northern Kingdom")));
        Birthplace->AddDynamicOption(TEXT("SouthernEmpire"), FText::FromString(TEXT("Southern Empire")));
        Birthplace->AddDynamicOption(TEXT("EasternLands"), FText::FromString(TEXT("Eastern Lands")));
        Birthplace->AddDynamicOption(TEXT("WesternFrontier"), FText::FromString(TEXT("Western Frontier")));
        Birthplace->AddDynamicOption(TEXT("CentralPlains"), FText::FromString(TEXT("Central Plains")));
        Birthplace->SetDefaultValueFromString(TEXT("CentralPlains"));

        Birthplace->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentBirthplace)));
        Birthplace->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentBirthplace)));
        Birthplace->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(Birthplace);
    }

    // 种族
    {
        UListDataObject_String* Race = NewObject<UListDataObject_String>();
        Race->SetDataID(FName("RaceOption"));
        Race->SetDisplayNameLocalizationKey(TEXT("DN_Race"));
        Race->SetDescriptionLocalizationKey(TEXT("RaceDesc"));

        Race->AddDynamicOption(TEXT("Human"), FText::FromString(TEXT("Human")));
        Race->AddDynamicOption(TEXT("Elf"), FText::FromString(TEXT("Elf")));
        Race->AddDynamicOption(TEXT("Dwarf"), FText::FromString(TEXT("Dwarf")));
        Race->AddDynamicOption(TEXT("Orc"), FText::FromString(TEXT("Orc")));
        Race->AddDynamicOption(TEXT("Beastkin"), FText::FromString(TEXT("Beastkin")));
        Race->SetDefaultValueFromString(TEXT("Human"));

        Race->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentRace)));
        Race->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentRace)));
        Race->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(Race);
    }

    // 性格
    {
        UListDataObject_String* Personality = NewObject<UListDataObject_String>();
        Personality->SetDataID(FName("PersonalityOption"));
        Personality->SetDisplayNameLocalizationKey(TEXT("DN_Personality"));
        Personality->SetDescriptionLocalizationKey(TEXT("PersonalityDesc"));

        Personality->AddDynamicOption(TEXT("Brave"), FText::FromString(TEXT("Brave")));
        Personality->AddDynamicOption(TEXT("Cunning"), FText::FromString(TEXT("Cunning")));
        Personality->AddDynamicOption(TEXT("Wise"), FText::FromString(TEXT("Wise")));
        Personality->AddDynamicOption(TEXT("Kind"), FText::FromString(TEXT("Kind")));
        Personality->AddDynamicOption(TEXT("Ambitious"), FText::FromString(TEXT("Ambitious")));
        Personality->SetDefaultValueFromString(TEXT("Brave"));

        Personality->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentPersonality)));
        Personality->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentPersonality)));
        Personality->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(Personality);
    }

    // 种族天赋
    {
        UListDataObject_String* RacialTalent = NewObject<UListDataObject_String>();
        RacialTalent->SetDataID(FName("RacialTalentOption"));
        RacialTalent->SetDisplayNameLocalizationKey(TEXT("DN_RacialTalent"));
        RacialTalent->SetDescriptionLocalizationKey(TEXT("RacialTalentDesc"));

        RacialTalent->AddDynamicOption(TEXT("NightVision"), FText::FromString(TEXT("Night Vision")));
        RacialTalent->AddDynamicOption(TEXT("FireResistance"), FText::FromString(TEXT("Fire Resistance")));
        RacialTalent->AddDynamicOption(TEXT("StrengthBonus"), FText::FromString(TEXT("Strength Bonus")));
        RacialTalent->AddDynamicOption(TEXT("AgilityBonus"), FText::FromString(TEXT("Agility Bonus")));
        RacialTalent->AddDynamicOption(TEXT("MagicAffinity"), FText::FromString(TEXT("Magic Affinity")));
        RacialTalent->SetDefaultValueFromString(TEXT("StrengthBonus"));

        RacialTalent->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentRacialTalent)));
        RacialTalent->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentRacialTalent)));
        RacialTalent->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(RacialTalent);
    }

    // 地区天赋
    {
        UListDataObject_String* RegionalTalent = NewObject<UListDataObject_String>();
        RegionalTalent->SetDataID(FName("RegionalTalentOption"));
        RegionalTalent->SetDisplayNameLocalizationKey(TEXT("DN_RegionalTalent"));
        RegionalTalent->SetDescriptionLocalizationKey(TEXT("RegionalTalentDesc"));

        RegionalTalent->AddDynamicOption(TEXT("ColdAdaptation"), FText::FromString(TEXT("Cold Adaptation")));
        RegionalTalent->AddDynamicOption(TEXT("HeatAdaptation"), FText::FromString(TEXT("Heat Adaptation")));
        RegionalTalent->AddDynamicOption(TEXT("TradeConnections"), FText::FromString(TEXT("Trade Connections")));
        RegionalTalent->AddDynamicOption(TEXT("SurvivalInstinct"), FText::FromString(TEXT("Survival Instinct")));
        RegionalTalent->SetDefaultValueFromString(TEXT("SurvivalInstinct"));

        RegionalTalent->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentRegionalTalent)));
        RegionalTalent->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentRegionalTalent)));
        RegionalTalent->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(RegionalTalent);
    }

    // 个人天赋
    {
        UListDataObject_String* PersonalTalent = NewObject<UListDataObject_String>();
        PersonalTalent->SetDataID(FName("PersonalTalentOption"));
        PersonalTalent->SetDisplayNameLocalizationKey(TEXT("DN_PersonalTalent"));
        PersonalTalent->SetDescriptionLocalizationKey(TEXT("PersonalTalentDesc"));

        PersonalTalent->AddDynamicOption(TEXT("Leadership"), FText::FromString(TEXT("Leadership")));
        PersonalTalent->AddDynamicOption(TEXT("Swordsmanship"), FText::FromString(TEXT("Swordsmanship")));
        PersonalTalent->AddDynamicOption(TEXT("Archery"), FText::FromString(TEXT("Archery")));
        PersonalTalent->AddDynamicOption(TEXT("Stealth"), FText::FromString(TEXT("Stealth")));
        PersonalTalent->AddDynamicOption(TEXT("Diplomacy"), FText::FromString(TEXT("Diplomacy")));
        PersonalTalent->SetDefaultValueFromString(TEXT("Swordsmanship"));

        PersonalTalent->SetDataDynamicGetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, GetCurrentPersonalTalent)));
        PersonalTalent->SetDataDynamicSetter(MakeControl(GET_FUNCTION_NAME_STRING_CHECKED(UPlayerSaveData, SetCurrentPersonalTalent)));
        PersonalTalent->SetShouldApplySettingsImmediately(true);

        CreateCharacterInfoTabCollection->AddChildListData(PersonalTalent);
    }

    RegisteredCharacterCreateTabCollections.Add(CreateCharacterInfoTabCollection);
}
