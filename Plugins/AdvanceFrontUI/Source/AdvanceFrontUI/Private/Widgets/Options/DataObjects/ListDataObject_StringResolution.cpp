// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

#include "FrontendUIDebugHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvaialbeResolutionss;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvaialbeResolutionss);

	AvaialbeResolutionss.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool {

			return A.SizeSquared() < B.SizeSquared();
		}

	);

	for (const FIntPoint& Resolution: AvaialbeResolutionss)
	{
		//Resolution Value from Dynamic Getter: (x=1920, y=1080)
		//FrontendUIDebugHelper::Log(TEXT("Available Resolution: ") + Resolution.ToString());
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResToValueString( AvaialbeResolutionss.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{

	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextByStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"),InResolution.X,InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString =   FString::Printf(TEXT("(%i x %i)"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
