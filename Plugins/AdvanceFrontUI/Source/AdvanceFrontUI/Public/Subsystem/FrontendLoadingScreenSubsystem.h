// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem,public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingReason);

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;

	/* Begin USystem Interface */
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize();
	/* End USystem Interface */

	/* Begin FTickableGameObject Interface */
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	/* End FTickableGameObject Interface */


private:
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);

	void OnMapPostLoaded(UWorld* LoadedWorld);

	void TryUpdataLoadingScreen();

	bool IsPreLoadScreenActive() const;

	bool ShouldShowLoadingScreen();

	bool CheckTheNeedToShowLoadingScreen();

	void TryDispalyLoadingScreenIfNone();

	void TryRemovedLoadingScreen();

	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);

	bool bIsCurrentlyLoadingMap = false;

	float HoldLoadingScreenStartUpTime = -1.f;

	FString CurrentLoadingReason;

	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;
};
