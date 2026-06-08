// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendStructTypes.generated.h"

/**
 * 
 */

USTRUCT()
struct FOptinsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc)
	{
		EditConditionFunc = InEditConditionFunc;
	}

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}

		return false;

	}
	FString GetDisabledRichReason() const { return DisabledRichReason; }

	void SetDisabledRichReason(const FString& InDisabledRichReason)
	{
		DisabledRichReason = InDisabledRichReason;
	}

	bool HasForcedStringVALUE() const
	{
		return DisabledForcedStringValue.IsSet();
	}
	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& InForcedValue)
	{
		DisabledForcedStringValue = InForcedValue;
	}

	
private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;

};