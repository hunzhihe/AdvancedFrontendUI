// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/FrontendPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"


void AFrontendPlayerController::OnPossess(APawn *InPawn)
{
    Super::OnPossess(InPawn);

    // 查找标记为 "MainMenuUICamera" 的摄像机并切换到该视角
    TArray<AActor*> Cameras;
    UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName("MainMenuUICamera"), Cameras);
    if (Cameras.Num() > 0)
    {
        ACameraActor* CameraActor = Cast<ACameraActor>(Cameras[0]);
        if (CameraActor)
        {
            SetViewTarget(CameraActor);
        }
    }

    // 首次运行时执行硬件基准测试以获取推荐的可扩展性设置
    UFrontendUIGameUserSettings* GameUserSetting =  UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings();

    if (GameUserSetting->GetLastCPUBenchmarkResult() == -1.f || GameUserSetting->GetLastGPUBenchmarkResult()==-1.f)
    {
        GameUserSetting->RunHardwareBenchmark();

        GameUserSetting->ApplyHardwareBenchmarkResults();
    }

}