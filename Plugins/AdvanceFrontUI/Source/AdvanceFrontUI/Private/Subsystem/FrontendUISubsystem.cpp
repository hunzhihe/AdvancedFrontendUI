// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendUIDebugHelper.h"
#include "Engine/AssetManager.h"
#include "Widgets/Widget_PrimaryLayout.h"
#include "Widgets/CommonActivatableWidgetContainer.h"



UFrontendUISubsystem* UFrontendUISubsystem::Get( const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UFrontendUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UFrontendUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(UFrontendUISubsystem::StaticClass(), FoundClasses, true);

		if (FoundClasses.Num() > 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Multiple classes derived from UFrontendUISubsystem found. Subsystem will not be created. Please ensure only one class derives from UFrontendUISubsystem."));
			return false;
		}
		return true;
	}

	return false;
}

void UFrontendUISubsystem::RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InPrimaryLayoutWidget)
{
	if (InPrimaryLayoutWidget)
	{
		CreatedPrimaryLayoutWidget = InPrimaryLayoutWidget;
		FrontendUIDebugHelper::Log(FString::Printf(TEXT("Registered Primary Layout Widget: %s"), *InPrimaryLayoutWidget->GetName()));
	}
}

void UFrontendUISubsystem::PushSoftWidgetToStackAynsc(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClassPtr,
	TFunction<void(EAsyncPushWidgetState, UWidget_ActivatableBase*)> AsyncPushStateCallBack)
{
	if (!SoftWidgetClassPtr.IsNull())
	{
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			SoftWidgetClassPtr.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([this, WidgetStackTag, SoftWidgetClassPtr, AsyncPushStateCallBack]()
				{
					UClass* LoadedClass = SoftWidgetClassPtr.Get();
					if (LoadedClass && CreatedPrimaryLayoutWidget)
					{
						FrontendUIDebugHelper::Log(FString::Printf(TEXT("Successfully loaded widget class: %s"), *LoadedClass->GetName()));
						UCommonActivatableWidgetContainerBase* FoundWidgetStack = CreatedPrimaryLayoutWidget->FindRegisteredWidgetStackByTag(WidgetStackTag);

						UWidget_ActivatableBase* CreatedWidget =  FoundWidgetStack->AddWidget<UWidget_ActivatableBase>(
							LoadedClass,
							[AsyncPushStateCallBack](UWidget_ActivatableBase& CreatedWidgetInstance){
								AsyncPushStateCallBack(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
							}
							
						);
						AsyncPushStateCallBack(EAsyncPushWidgetState::AfterPush, CreatedWidget);

					}
					else
					{
						FrontendUIDebugHelper::Log(FString::Printf(TEXT("Failed to load widget class for tag: %s"), *WidgetStackTag.ToString()));
					}
				})



		);
	}
}
		