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
	static UFrontendUIGameUserSettings* GetFrontendUIGameUserSettings();

	//GameplayCollectionTab
	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InDifficulty) { CurrentGameDifficulty = InDifficulty; }

	//GameplayCollectionTab
	
private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
};
