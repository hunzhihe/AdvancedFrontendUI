// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendSettings/FrontendUIGameUserSettings.h"

UFrontendUIGameUserSettings* UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()
{

	if (GEngine)
	{
		return CastChecked<UFrontendUIGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}
