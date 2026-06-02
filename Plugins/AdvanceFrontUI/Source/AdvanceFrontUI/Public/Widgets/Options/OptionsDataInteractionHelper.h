// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"


class UFrontendUIGameUserSettings;
/**
 * 
 */
class ADVANCEFRONTUI_API FOptionsDataInteractionHelper
{
public:
	FOptionsDataInteractionHelper(const FString& InSettterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueFromString(const FString& InValue) const;

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UFrontendUIGameUserSettings> CachedWeakGameUserSettings;
	
};
