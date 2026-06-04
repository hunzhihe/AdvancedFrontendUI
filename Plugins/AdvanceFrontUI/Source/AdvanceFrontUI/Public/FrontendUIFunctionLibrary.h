// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "FrontendUIFunctionLibrary.generated.h"


class UWidget_ActivatableBase;
class UTexture2D;

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:

	UFUNCTION(BlueprintPure, Category = "Frontend UI Functions library")
	static TSoftClassPtr<UWidget_ActivatableBase> GetFrontendWidgetFromTag(
	 UPARAM(meta = (Categories = "Frontend.Widget"))FGameplayTag WidgetTag);

	UFUNCTION(BlueprintPure, Category = "Frontend UI Functions library")
	static TSoftObjectPtr<UTexture2D>GetOptionsSoftImage(
		UPARAM(meta = (Categories = "Frontend.Image"))FGameplayTag ImageTag);
};
