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

	
	virtual bool CanSetToForcedStringvalue(const FString& InForcedValue) const override;
	
	virtual void  OnSetToForcedStringvalue(const FString& InForcedValue) override;
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


UCLASS()
class ADVANCEFRONTUI_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()

public:
	template<typename EnumType>
	void AddEnumOptions(EnumType InEnumOption, const FText& InDispalyText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConveretedEnumString =  StaticEnumOption->GetNameStringByValue(InEnumOption);

		AddDynamicOption(ConveretedEnumString, InDispalyText);
		
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue);
	}

	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		const FString ConveretedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConveretedEnumString);
	}
};


UCLASS()
class ADVANCEFRONTUI_API UListDataObject_StringInteger : public UListDataObject_String
{

	GENERATED_BODY()

public:
	void AddIntegerOption(int32 InIntegerValue, const FText& InDispalyText);

protected:

	//Begin UListDataObject_String interface
	virtual void OnDataObjectInitialized() override;

	virtual void OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason) override;
	//End UListDataObject_String interface
private:



};