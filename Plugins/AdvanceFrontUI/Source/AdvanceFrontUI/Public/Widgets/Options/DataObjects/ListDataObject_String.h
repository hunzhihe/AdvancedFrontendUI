// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/MyListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_String : public UMyListDataObject_Value
{
	GENERATED_BODY()

public:

	void AddDynamicOption(const FString& OptionString, const FText& OptionDisplayText);

	void AdvanceToNextOption();
	void BackToPerviousOption();

	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);

protected:
	//Begin UListDataObject_Base interface
	virtual void OnDataObjectInitialized() override;
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue()  override;
	//End UListDataObject_Base interface

	bool TrySetDisplayTextByStringValue(const FString& InStringValue);

	FString CurrentStringValue;
	FText CurrentDisplayText;
	TArray<FString> AvaiableOptionsStringArray;
	TArray<FText> AvaiableOptionsDisplayTextArray;

public:

	FORCEINLINE const TArray<FText>& GetAvaiableOptionsDisplayTextArray() const { return AvaiableOptionsDisplayTextArray; }
	FORCEINLINE const FString& GetCurrentStringValue() const { return CurrentStringValue; }
	FORCEINLINE const FText& GetCurrentDisplayText() const { return CurrentDisplayText; }
	FORCEINLINE const TArray<FString>& GetAvaiableOptionsStringArray() const { return AvaiableOptionsStringArray; }
	
};


//UCLASS()
//class ADVANCEFRONTUI_API UListDataObject_Stringbool : public UListDataObject_String
//{
//	GENERATED_BODY()
//
//
//public:
//	void OverrideTrueDisplayText(const FText& InNameTrueDisplayText);
//	void OverrideFalseDisplayText(const FText& InNameFalseDisplayText);
//
//	void SetTrueAsDefaultValue();
//	void SetFalseAsDefaultValue();
//
//protected:
//
//	//UListDataObject_String Interface
//	virtual void OnDataObjectInitialized() override;
//
//	//UListDataObject_String Interface
//
//private:
//	void TryInitBoolValue();
//
//	const FString TrusString = TEXT("true");
//	const FString FalseString = TEXT("false");
//
//};