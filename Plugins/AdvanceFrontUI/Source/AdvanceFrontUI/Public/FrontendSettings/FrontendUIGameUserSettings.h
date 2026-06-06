// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendUIGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUIGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:	
	UFrontendUIGameUserSettings();

	static UFrontendUIGameUserSettings* GetFrontendUIGameUserSettings();

	//GameplayCollectionTab
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InDifficulty) { CurrentGameDifficulty = InDifficulty; }

	//GameplayCollectionTab



	//Audio Collection Tab
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

	//Audio Collection Tab
	
private:

	//Gameplay Collection Tab
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	//Gameplay Collection Tab


	//Audio Collection Tab
	UPROPERTY(Config)
	float OverallVolume;

	UPROPERTY(Config)
	float MusicVolume;

	UPROPERTY(Config)
	float SoundFXVolume;

	UPROPERTY(Config)
	bool bAllowBackgroundAudio;



	//Audio Collection Tab
};
