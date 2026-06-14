// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game,DefaultConfig)
class ADVANCEFRONTUI_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Setting")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Setting")
	float HoldLoadingScreenExtraSconds = 3.f;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Setting")
	bool bShouldLoadingScreenInEditor = false;

};
