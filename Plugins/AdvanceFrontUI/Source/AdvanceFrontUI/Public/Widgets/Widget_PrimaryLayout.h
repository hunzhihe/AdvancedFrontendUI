// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "Widget_PrimaryLayout.generated.h"

/**
 * 
 */
class UCommonActivatableWidgetContainerBase;

UCLASS(Abstract, BlueprintType,meta=(DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_PrimaryLayout : public UCommonUserWidget
{
	GENERATED_BODY()
public:
    UCommonActivatableWidgetContainerBase* FindRegisteredWidgetStackByTag(const FGameplayTag& WidgetStackTag) const;
	

protected:
	
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta=(Category = "Frontend.WidgetStack")) const FGameplayTag& WidgetStackTag,UCommonActivatableWidgetContainerBase* WidgetStack);

	// UFUNCTION(BlueprintCallable)
	// void UnregisterWidgetStack(const FGameplayTag& WidgetStackTag);

	// UFUNCTION(BlueprintCallable)
	// UCommonActivatableWidgetContainerBase* GetRegisteredWidgetStack(const FGameplayTag& WidgetStackTag) const;

private:
    UPROPERTY(Transient)
	TMap<FGameplayTag,UCommonActivatableWidgetContainerBase*> RegisteredWidgetStackMap;

	
};
