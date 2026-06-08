// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "ListDataObject_StringResolution.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_StringResolution : public UListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();


protected:
	//Begin UListDataObject_String interface
	virtual void OnDataObjectInitialized() override;

	//Begin UListDataObject_String interface

private:
	FString ResToValueString(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaximumAllowedResolution;

public:
	FORCEINLINE FString GetMaximumAllowedResolution() const {
		return MaximumAllowedResolution;
	}
};
