// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "MyListDataObject_Value.generated.h"


class FOptionsDataInteractionHelper;
/**
 * 
 */
UCLASS(Abstract)
class ADVANCEFRONTUI_API UMyListDataObject_Value : public UListDataObject_Base
{
	GENERATED_BODY()
public:
	void SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter);
	void SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter);

	void SetDefaultValueFromString(const FString& InDefaultValue) { DefaultStringValue = InDefaultValue; }


	virtual bool HasDefaultValue() const override { return DefaultStringValue.IsSet(); }


protected:
	FString GetDefualtValueAsString() const { return DefaultStringValue.GetValue(); }

	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicGetter;
	TSharedPtr<FOptionsDataInteractionHelper> DataDynamicSetter;

private:
	TOptional<FString> DefaultStringValue;
	
};
