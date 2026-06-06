// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_Stringbool.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_Stringbool : public UListDataObject_String
{
	GENERATED_BODY()


public:
	void OverrideTrueDisplayText(const FText& InNameTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNameFalseDisplayText);

	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

protected:

	//UListDataObject_String Interface
	virtual void OnDataObjectInitialized() override;

	//UListDataObject_String Interface

private:
	void TryInitBoolValue();

	const FString TrusString = TEXT("true");
	const FString FalseString = TEXT("false");
	
};
