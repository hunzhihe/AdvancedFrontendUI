// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendSettings/FrontendUIGameUserSettings.h"
#include "Internationalization/Internationalization.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "Engine/GameInstance.h"

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
	// 从 GEngine 中获取游戏用户设置单例
	if (GEngine)
	{
		return CastChecked<UFrontendUIGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}


static EWindowMode::Type GetPlatformFullscreenMode(int InFullscreenMode)
{
	EWindowMode::Type Mode = EWindowMode::ConvertIntToWindowMode(InFullscreenMode);

	return (!FPlatformProperties::SupportsWindowedMode()) ? EWindowMode::Fullscreen : Mode;
}

static FString GetCultureCodeForLanguage(ELaughageChanged Language)
{
	switch (Language)
	{
	case ELaughageChanged::English: return TEXT("en");
	case ELaughageChanged::ZH_Ch:   return TEXT("zh-Hans");
	default:                        return TEXT("");
	}
}



void UFrontendUIGameUserSettings::SetCurrentLanguage(const ELaughageChanged InCurrentLanguage)
{
	// 1. 存储设置值
	CurrentGameLaughage = InCurrentLanguage;
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

float UFrontendUIGameUserSettings::GetCurrentDisplayGamma() const
{
	// 从 GEngine 读取当前的显示伽马值
	if (GEngine)
	{
		return GEngine->GetDisplayGamma();
	}

	return 0.0f;
}

void UFrontendUIGameUserSettings::SetCurrentDisplayGamma(const float& InDisplayGamma)
{
	// 直接修改 GEngine 的显示伽马值
	if (GEngine)
	{
		GEngine->DisplayGamma = InDisplayGamma;
	}

}
