// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "InteractionWidget.generated.h"

/**
 * 
 */

class UManager_InteractionTarget;

UCLASS(Abstract, BlueprintType)
class ADVANCEFRONTUI_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void UpdateInteractionTargets(UManager_InteractionTarget* InteractionTarget);
	//IsInteraction
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void UpdateContentStates(bool IsInteraction);
	//FKey Key
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void SetInteractionKeyTexts(FKey Key);
	//IsNewTarget
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnWidgetNewTargets(bool IsNewTarget);
	
};
