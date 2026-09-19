// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HUDInteractionBPL.generated.h"

/**
 * 交互界面数学工具库（纯函数，供蓝图调用）
 */
UCLASS()
class ADVANCEFRONTUI_API UHUDInteractionBPL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    /**
     * 判断屏幕位置是否在视口内（并考虑动态边距）
     * 完全复刻蓝图 IsOnScreen? 的 SelectFloat + InRange 逻辑
     * @param ScreenPosition   屏幕坐标（像素）
     * @param ViewportSize     视口尺寸
     * @param Margin           边距（Left/Right/Top/Bottom）
     * @return                 是否在视口内（包含边距）
     */
    UFUNCTION(BlueprintPure, Category = "Interaction|Math")
    static bool IsScreenPositionInViewport(const FVector2D& ScreenPosition,
        const FVector2D& ViewportSize,
        const FMargin& Margin);

    /**
     * 计算目标在屏幕外时的“边缘世界坐标”，并将其重新投影到屏幕
     * 完全复刻蓝图 Update_Position_In_Viewport 中屏幕外分支的复杂算法
     * @param TargetWorldLocation   目标世界位置（对应 LastWorldLocation）
     * @param CameraWorldLocation   摄像机世界位置（对应 LocCamLoc）
     * @param PlayerController      玩家控制器（用于投影）
     * @param ScreenRadiusPercent   屏幕半径百分比（对应 ScreenRadiusPercent）
     * @return                      边缘位置对应的屏幕坐标（像素）
     */
    UFUNCTION(BlueprintPure, Category = "Interaction|Math")
    static FVector2D ComputeEdgeScreenPosition(const FVector& TargetWorldLocation,
        const FVector& CameraWorldLocation,
        APlayerController* PlayerController,
        float ScreenRadiusPercent);


    /**
     * "屏幕外"分支的数学逻辑
     * 返回一个位于屏幕边缘的指示器位置（屏幕坐标）
     */
    UFUNCTION(BlueprintPure, Category = "Interaction|Math")
    static FVector2D GetEdgeIndicatorScreenPosition(
        const FVector& TargetWorldLocation,   // 即蓝图的 LastWorldLocation
        const FVector& CameraLocation,        // 摄像机的世界位置
        const FRotator& CameraRotation,       // 摄像机的世界旋转
        APlayerController* PlayerController,  // 用于 ProjectWorldLocationToScreen
        const FVector2D& ViewportSize,        // 视口大小
        float ScreenRadiusPercent             // 你的变量 ScreenRadiusPercent
    );

};
