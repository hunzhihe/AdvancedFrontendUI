// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/FrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"


void AFrontendPlayerController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    TArray<AActor*> Cameras;
    // Set the input mode to UI only and show the mouse cursor
    UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("MainMenuUICamera"), Cameras);
    if (Cameras.Num() > 0)
    {
        ACameraActor* CameraActor = Cast<ACameraActor>(Cameras[0]);
        if (CameraActor)
        {
            SetViewTarget(CameraActor);
        }
    }

    UFrontendUIGameUserSettings* GameUserSetting =  UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings();

    if (GameUserSetting->GetLastCPUBenchmarkResult() == -1.f || GameUserSetting->GetLastGPUBenchmarkResult()==-1.f)
    {
        GameUserSetting->RunHardwareBenchmark();

        GameUserSetting->ApplyHardwareBenchmarkResults();
    }

}