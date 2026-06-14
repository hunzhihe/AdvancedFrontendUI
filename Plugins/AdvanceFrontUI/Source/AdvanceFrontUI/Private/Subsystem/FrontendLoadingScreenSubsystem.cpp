// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/FrontendLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interface/FrontendUILoadScreenInterface.h"

#include "FrontendUIDebugHelper.h"

bool UFrontendLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 非专用服务器且在无派生类时创建
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(),FoundClasses);

		return FoundClasses.IsEmpty();
	}
	return false;
}

void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	// 监听地图预加载和加载完成事件
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapPostLoaded);
}

void UFrontendLoadingScreenSubsystem::Deinitialize()
{
	// 移除所有事件绑定
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

}

UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}

	return nullptr;
}

void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
{

	TryUpdataLoadingScreen();
	//FrontendUIDebugHelper::Log(TEXT("Ticking"));
}

ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}

	return ETickableTickType::Conditional;
}

bool UFrontendLoadingScreenSubsystem::IsTickable() const
{
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient(); ;
}

TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem,STATGROUP_Tickables);
}

void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}
	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdataLoadingScreen();
	//FrontendUIDebugHelper::Log(TEXT("On Map Pre Loaded"));
}

void UFrontendLoadingScreenSubsystem::OnMapPostLoaded( UWorld* LoadedWorld)
{
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}

	//FrontendUIDebugHelper::Log(TEXT("On Map Post Loaded"));
}

void UFrontendLoadingScreenSubsystem::TryUpdataLoadingScreen()
{
	//check if there's any up loading screen that is currently active
	if (IsPreLoadScreenActive())
	{
		return;
	}


	//check if we should show the loading here

	if (ShouldShowLoadingScreen())
	{
		//try display the loading screen here

		TryDispalyLoadingScreenIfNone();

		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		//try remove the current active loading screen

		TryRemovedLoadingScreen();

		HoldLoadingScreenStartUpTime = -1.f;
		//notify the loading is complete

		NotifyLoadingScreenVisibilityChanged(false);

		//disable the ticking
		SetTickableTickType(ETickableTickType::Never);
	}

}

bool UFrontendLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreLoadScreenManager = FPreLoadScreenManager::Get())
	{
	   return	PreLoadScreenManager->HasValidActivePreLoadScreen();

	}

	return false;
}

bool UFrontendLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();

	if (GIsEditor && !LoadingScreenSettings->bShouldLoadingScreenInEditor)
	{
		return false;
	}

	//check if the objects in the world need a loading screen
	if (CheckTheNeedToShowLoadingScreen())
	{
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;

		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");

	//there is no need to show the loading screen, allow the world to be render to our viewport here
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	const float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartUpTime<0.f)
	{
		HoldLoadingScreenStartUpTime = CurrentTime;
	}

	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartUpTime;

	if (ElapsedTime<LoadingScreenSettings->HoldLoadingScreenExtraSconds)
	{
		return true;
	}

	return false;
}

bool UFrontendLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	UWorld* OwningWorld = GetGameInstance()->GetWorld();

	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");

		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World has not Begun play yet");

		return true;
	}

	if (!OwningWorld->GetFirstLocalPlayerFromController())
	{
		CurrentLoadingReason = TEXT("Player Controller is not valid yet");

		return true;
	}

	//check if the game state, player states,or player character, actor component are ready.
	return false;
}

void UFrontendLoadingScreenSubsystem::TryDispalyLoadingScreenIfNone()
{

	//if there is already loading screen, return aerly if yes
    
	if (CachedCreatedLoadingScreenWidget)
	{
		return;
	}

	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();

	TSubclassOf<UUserWidget>LoadingWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	UUserWidget* CreatedWidgte =  UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadingWidgetClass, NAME_None);

	check(CreatedWidgte);

	CachedCreatedLoadingScreenWidget = CreatedWidgte->TakeWidget();

	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreatedLoadingScreenWidget.ToSharedRef(),
		1000
	);

	NotifyLoadingScreenVisibilityChanged(true);
	
}

void UFrontendLoadingScreenSubsystem::TryRemovedLoadingScreen()
{
	if (!CachedCreatedLoadingScreenWidget)
	{
		return;
	}
	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());

	CachedCreatedLoadingScreenWidget.Reset();

	
}

void UFrontendLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer)
		{
			continue;
		}

		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			//Query if the playercontroller implements the interface, Call the function through interface to notify the loading status if yes.
			if (PC->Implements<UFrontendUILoadScreenInterface>())
			{
				if (bIsVisible) 
				{
					IFrontendUILoadScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IFrontendUILoadScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}

			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UFrontendUILoadScreenInterface>())
				{
					if (bIsVisible)
					{
						IFrontendUILoadScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IFrontendUILoadScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}
		}

		//The code for notifying other objects in the world goes here

	}
}
