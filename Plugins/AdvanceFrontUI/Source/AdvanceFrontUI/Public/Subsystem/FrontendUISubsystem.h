// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <Widgets/Widget_ActivatableBase.h>
#include "FrontendUISubsystem.generated.h"



class UWidget_PrimaryLayout;
struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush

};
/**
 * 
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	static UFrontendUISubsystem* Get(const UObject* WorldContextObject);

	// Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// End USubsystem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UWidget_PrimaryLayout* InPrimaryLayoutWidget);

	void PushSoftWidgetToStackAynsc(const FGameplayTag& WidgetStackTag,TSoftClassPtr<UWidget_ActivatableBase> SoftWidgetClassPtr,TFunction<void(EAsyncPushWidgetState,UWidget_ActivatableBase*)> AsyncPushStateCallBack);

private:
	UPROPERTY(Transient)
	UWidget_PrimaryLayout* CreatedPrimaryLayoutWidget;
};
