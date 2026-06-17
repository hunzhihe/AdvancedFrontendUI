// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "FrontendUIGameUserSettings.generated.h"

/**
 * 前端 UI 游戏用户设置
 * 继承自 UGameUserSettings，管理前端 UI 相关的持久化用户偏好设置：
 * - Gameplay: 游戏难度
 * - Audio: 主音量、音乐音量、音效音量、后台音频开关、HDR 音频开关
 * - Video: 显示伽马值
 * 通过 FOptionsDataInteractionHelper 的 PropertyPath 机制与选项界面数据对象桥接。
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUIGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFrontendUIGameUserSettings();

	/** 获取前端游戏用户设置的单例实例 */
	static UFrontendUIGameUserSettings* GetFrontendUIGameUserSettings();

	// ========== Gameplay 选项卡 ==========
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InDifficulty) { CurrentGameDifficulty = InDifficulty; }

	UFUNCTION()
	ELaughageChanged GetCurrentLanguage() const { return CurrentGameLaughage; }

	UFUNCTION()
	void SetCurrentLanguage(const ELaughageChanged InCurrentLanguage);
	// ========== Gameplay 选项卡 ==========



	// ========== Audio 选项卡 ==========
	UFUNCTION()
	float GetCurrentOverallVolume() const { return OverallVolume;}

	UFUNCTION()
	void SetCurrentOverallVolume(const float& InOverallVolume);

	UFUNCTION()
	float GetCurrentMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetCurrentMusicVolume(const float& InMusicVolume);

	UFUNCTION()
	float GetCurrentSoundFXVolume() const { return SoundFXVolume; }

	UFUNCTION()
	void SetCurrentSoundFXVolume(const float& InSoundFXVolume);

	UFUNCTION()
	bool GetCurrentAllowBackgroundAudio() const { return bAllowBackgroundAudio; }

	UFUNCTION()
	void SetCurrentAllowBackgroundAudio( bool InAllowBackgroundAudio);

	UFUNCTION()
	bool GetCurrentUserHDRAudio() const { return bUserHDRAudio; }

	UFUNCTION()
	void SetCurrentUserHDRAudio(bool InUserHDRAudio);
	// ========== Audio 选项卡 ==========



	// ========== Video 选项卡 ==========
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(const float& InDisplayGamma);
	// ========== Video 选项卡 ==========

private:

	// ========== Gameplay 选项卡数据 ==========
	UPROPERTY(Config)
	FString CurrentGameDifficulty;

	UPROPERTY(Config)
	ELaughageChanged CurrentGameLaughage;

	// ========== Gameplay 选项卡数据 ==========


	// ========== Audio 选项卡数据 ==========
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SoundFXVolume;

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;

	UPROPERTY(Config)
	bool bUserHDRAudio;
	// ========== Audio 选项卡数据 ==========


	// ========== Video 选项卡数据 ==========

	// ========== Video 选项卡数据 ==========

};
