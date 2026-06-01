// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendUIButtonInternalBase.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUIButtonInternalBase : public UCommonButtonInternalBase
{
	GENERATED_BODY()

public:
	TSharedPtr<class SCommonButton> GetSlateButton() const { return MyCommonButton; }
	
};
