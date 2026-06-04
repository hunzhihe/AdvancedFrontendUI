// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendDeveloperSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Frontend UI Settings"))
class ADVANCEFRONTUI_API UFrontendDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	UPROPERTY(Config, EditAnywhere, Category = "Widget References", meta = (ForceInlineRow, Categories = "Frontend.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UWidget_ActivatableBase>> FrontendWidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Option Image References", meta = (ForceInlineRow, Categories = "Frontend.Iamge"))
	TMap< FGameplayTag, TSoftObjectPtr<UTexture2D>> OptionsScreenSoftImageMap;
};
