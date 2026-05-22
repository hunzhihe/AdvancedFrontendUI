// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/FrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"

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
}