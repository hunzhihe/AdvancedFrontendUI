// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendSettings/FrontendUIGameUserSettings.h"

UFrontendUIGameUserSettings::UFrontendUIGameUserSettings()
	:OverallVolume(1.F)
	,MusicVolume(1.f)
	, SoundFXVolume(1.f)
	, bAllowBackgroundAudio(false)
	, bUserHDRAudio(false)
{
}

UFrontendUIGameUserSettings* UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()
{

	if (GEngine)
	{
		return CastChecked<UFrontendUIGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendUIGameUserSettings::SetCurrentOverallVolume(const float& InOverallVolume)
{
	OverallVolume = InOverallVolume;

	//The actual Logic for controlling the volume goes here
}

void UFrontendUIGameUserSettings::SetCurrentMusicVolume(const float& InMusicVolume)
{
	MusicVolume = InMusicVolume;
}

void UFrontendUIGameUserSettings::SetCurrentSoundFXVolume(const float& InSoundFXVolume)
{
	SoundFXVolume = InSoundFXVolume;
}

void UFrontendUIGameUserSettings::SetCurrentAllowBackgroundAudio( bool InAllowBackgroundAudio)
{
	bAllowBackgroundAudio = InAllowBackgroundAudio;
}

void UFrontendUIGameUserSettings::SetCurrentUserHDRAudio(bool InUserHDRAudio)
{
	bUserHDRAudio = InUserHDRAudio;
}
