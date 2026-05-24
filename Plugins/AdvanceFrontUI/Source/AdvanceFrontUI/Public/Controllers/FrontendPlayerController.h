// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrontendPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API AFrontendPlayerController : public APlayerController
{
	GENERATED_BODY()
	protected:

	//
	virtual void OnPossess(APawn* InPawn) override;
};
