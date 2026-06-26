// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveData.generated.h"

/**
 * 玩家存档数据
 * 通过 USaveGame 持久化到磁盘（Saved/SaveGames/），支持多存档槽位。
 * 和 UFrontendUIGameUserSettings（保存设置到 .ini）分离开，
 * 专门存储角色创建数据、游戏进度等存档内容。
 */
UCLASS()
class ADVANCEFRONTUI_API UPlayerSaveData : public USaveGame
{
    GENERATED_BODY()

public:
    // ========== 角色基础信息 ==========

    /** 角色名称 */
    UFUNCTION()
    FString GetCurrentCharacterName() const { return CharacterName; }
    UFUNCTION()
    void SetCurrentCharacterName(const FString& InCharacterName) { CharacterName = InCharacterName; }

    /** 角色性别 */
    UFUNCTION()
    FString GetCurrentCharacterSex() const { return CharacterSex; }
    UFUNCTION()
    void SetCurrentCharacterSex(const FString& InCharacterSex) { CharacterSex = InCharacterSex; }

    /** 出生地 */
    UFUNCTION()
    FString GetCurrentBirthplace() const { return Birthplace; }
    UFUNCTION()
    void SetCurrentBirthplace(const FString& InBirthplace) { Birthplace = InBirthplace; }

    /** 种族 */
    UFUNCTION()
    FString GetCurrentRace() const { return Race; }
    UFUNCTION()
    void SetCurrentRace(const FString& InRace) { Race = InRace; }

    /** 性格 */
    UFUNCTION()
    FString GetCurrentPersonality() const { return Personality; }
    UFUNCTION()
    void SetCurrentPersonality(const FString& InPersonality) { Personality = InPersonality; }

    /** 种族天赋 */
    UFUNCTION()
    FString GetCurrentRacialTalent() const { return RacialTalent; }
    UFUNCTION()
    void SetCurrentRacialTalent(const FString& InRacialTalent) { RacialTalent = InRacialTalent; }

    /** 地区天赋 */
    UFUNCTION()
    FString GetCurrentRegionalTalent() const { return RegionalTalent; }
    UFUNCTION()
    void SetCurrentRegionalTalent(const FString& InRegionalTalent) { RegionalTalent = InRegionalTalent; }

    /** 个人天赋 */
    UFUNCTION()
    FString GetCurrentPersonalTalent() const { return PersonalTalent; }
    UFUNCTION()
    void SetCurrentPersonalTalent(const FString& InPersonalTalent) { PersonalTalent = InPersonalTalent; }

    // ========== 外观参数 ==========

    /** 选择的职业 */
    UPROPERTY()
    FName SelectedClass;

    /** 外观参数（肤色、发型、体型等，键为参数名，值为 0~1 范围内的浮点数） */
    UPROPERTY()
    TMap<FName, float> AppearanceParams;

    /** 角色创建时间戳 */
    UPROPERTY()
    FDateTime CreationTimestamp;

private:
    /** 角色名称 */
    UPROPERTY()
    FString CharacterName;

    /** 角色性别 */
    UPROPERTY()
    FString CharacterSex;

    /** 出生地 */
    UPROPERTY()
    FString Birthplace;

    /** 种族 */
    UPROPERTY()
    FString Race;

    /** 性格 */
    UPROPERTY()
    FString Personality;

    /** 种族天赋 */
    UPROPERTY()
    FString RacialTalent;

    /** 地区天赋 */
    UPROPERTY()
    FString RegionalTalent;

    /** 个人天赋 */
    UPROPERTY()
    FString PersonalTalent;
};
