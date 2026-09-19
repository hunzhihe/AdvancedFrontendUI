// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDInteractionManager/HUDInteractionBPL.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

bool UHUDInteractionBPL::IsScreenPositionInViewport(const FVector2D& ScreenPosition, const FVector2D& ViewportSize, const FMargin& Margin)
{
    if (ViewportSize.IsNearlyZero())
    {
        return false;
    }

    const float HalfX = ViewportSize.X * 0.5f;
    const float HalfY = ViewportSize.Y * 0.5f;

    // 复刻蓝图中的 SelectFloat 逻辑：
    // 若 X < 半宽，则取 Left，否则取 Right
    const float MinX = (ScreenPosition.X < HalfX) ? Margin.Left : Margin.Right;
    const float MinY = (ScreenPosition.Y < HalfY) ? Margin.Top : Margin.Bottom;

    const float MaxX = ViewportSize.X - MinX;
    const float MaxY = ViewportSize.Y - MinY;

    // 对应蓝图中的两个 InRange_FloatFloat + AND
    return (ScreenPosition.X >= MinX && ScreenPosition.X <= MaxX &&
        ScreenPosition.Y >= MinY && ScreenPosition.Y <= MaxY);
}

FVector2D UHUDInteractionBPL::ComputeEdgeScreenPosition(const FVector& TargetWorldLocation, const FVector& CameraWorldLocation, APlayerController* PlayerController, float ScreenRadiusPercent)
{
    if (!PlayerController || ScreenRadiusPercent <= 0.0f)
    {
        return FVector2D::ZeroVector;
    }

    // 1. 将目标投影到屏幕，获取方向基准
    FVector2D ProjectedScreenPos;
    bool bSuccess = PlayerController->ProjectWorldLocationToScreen(TargetWorldLocation, ProjectedScreenPos, true);
    if (!bSuccess)
    {
        return FVector2D::ZeroVector;
    }

    // 2. 获取视口尺寸与中心
    int32 ViewportX = 0, ViewportY = 0;
    PlayerController->GetViewportSize(ViewportX, ViewportY);
    const FVector2D ViewportSize(static_cast<float>(ViewportX), static_cast<float>(ViewportY));
    if (ViewportSize.IsNearlyZero())
    {
        return ProjectedScreenPos;
    }

    const FVector2D ScreenCenter = ViewportSize * 0.5f;

    // 3. 计算从屏幕中心到投影点的归一化方向
    FVector2D ScreenDir = ProjectedScreenPos - ScreenCenter;
    const float Length = ScreenDir.Size();
    if (Length < 1.0f)
    {
        // 如果几乎在中心，直接返回投影位置（避免除零）
        return ProjectedScreenPos;
    }
    const FVector2D NormalizedDir = ScreenDir / Length;

    // 4. 计算半径（对应蓝图：视口高度 * ScreenRadiusPercent）
    const float Radius = ViewportSize.Y * ScreenRadiusPercent;

    // 5. 计算从摄像机指向目标的世界方向（对应 FindLookAtRotation）
    const FVector DirVec = (TargetWorldLocation - CameraWorldLocation).GetSafeNormal();
    const FRotator LookAtRot = DirVec.Rotation();

    // 6. 获取摄像机朝向的局部坐标轴（对应蓝图中的 GetForward/Right/UpVector）
    const FVector Forward = UKismetMathLibrary::GetForwardVector(LookAtRot);
    const FVector Right = UKismetMathLibrary::GetRightVector(LookAtRot);
    const FVector Up = UKismetMathLibrary::GetUpVector(LookAtRot);

    // 7. 计算世界偏移量标量
    // 注意：NormalizedDir.X = Cos(角度), NormalizedDir.Y = Sin(角度)
    const float ScalarX = Radius * NormalizedDir.X;  // 用于 Up 向量
    const float ScalarY = Radius * NormalizedDir.Y;  // 用于 Right 向量

    // 8. 计算边缘世界坐标（对应蓝图中的加法链）
    // EdgeWorld = CameraLoc + Forward + Up*ScalarX + Right*ScalarY
    const FVector EdgeWorldLocation = CameraWorldLocation + Forward + (Up * ScalarX) + (Right * ScalarY);

    // 9. 将边缘世界坐标重新投影到屏幕
    FVector2D EdgeScreenPos;
    bSuccess = PlayerController->ProjectWorldLocationToScreen(EdgeWorldLocation, EdgeScreenPos, true);
    if (!bSuccess)
    {
        // 保底：若投影失败，返回原始投影位置
        return ProjectedScreenPos;
    }

    return EdgeScreenPos;
}

FVector2D UHUDInteractionBPL::GetEdgeIndicatorScreenPosition(const FVector& TargetWorldLocation, const FVector& CameraLocation, const FRotator& CameraRotation, APlayerController* PlayerController, const FVector2D& ViewportSize, float ScreenRadiusPercent)
{
    if (!PlayerController)
    {
        return ViewportSize * 0.5f;
    }

    // ==================== 阶段 1: 计算偏移后的世界位置（优化版） ====================
    // 1.1 直接计算从摄像机指向目标的方向向量（避免 FindLookAtRotation 带来的 Atan2 开销）
    FVector DirToTarget = (TargetWorldLocation - CameraLocation).GetSafeNormal();

    // 1.2 计算摄像机前向
    FVector CamForward = CameraRotation.Vector();

    // 1.3 计算点积，并限定最大值 <= 0 （简化了蓝图中 InRange=0,OutRange=0 的 MapRangeClamped）
    float DotVal = FVector::DotProduct(CamForward, DirToTarget);
    float MappedVal = FMath::Min(0.0f, DotVal); // 蓝图逻辑：如果 Dot > 0，则值为 0；否则为 Dot

    // 1.4 构建局部坐标系（直接向量叉积，避免构造 Rotator）
    // 注意：这里假设世界向上为 (0,0,1)，与 FindLookAtRotation 内部行为一致
    FVector RightVec = FVector::CrossProduct(FVector::UpVector, DirToTarget).GetSafeNormal();
    FVector UpVec = FVector::CrossProduct(DirToTarget, RightVec); // 已经归一化

    // 1.5 计算偏移后的世界位置: CameraLocation + Dir + Up*Mapped + Right
    FVector OffsetWorldLoc = CameraLocation + DirToTarget + (UpVec * MappedVal) + RightVec;

    // ==================== 阶段 2: 投影到屏幕（无法优化，保留引擎接口） ====================
    FVector2D ScreenPos;
    bool bProjected = PlayerController->ProjectWorldLocationToScreen(OffsetWorldLoc, ScreenPos, true);
    if (!bProjected)
    {
        ScreenPos = ViewportSize * 0.5f;
    }

    // ==================== 阶段 3: 计算边缘指示器位置（优化版） ====================
    FVector2D HalfViewport = ViewportSize * 0.5f;
    FVector2D DirVec = ScreenPos - HalfViewport;
    float Length = DirVec.Size();

    if (Length < KINDA_SMALL_NUMBER)
    {
        return HalfViewport;
    }

    // 3.1 归一化方向向量
    FVector2D Norm = DirVec / Length;

    // 3.2 优化核心：消除冗余的 Acos + Cos/Sin
    // 原蓝图: Cos(Acos(X)) = X，Sin(Acos(Y)) = Sqrt(1 - Y^2)
    float CosVal = Norm.X;
    float SinVal = FMath::Sqrt(1.0f - Norm.Y * Norm.Y); // 因为 Acos(Y) 范围 [0, PI]，Sin 恒 >= 0

    // 3.3 根据 Y 分量符号决定 Sin 的正负（与蓝图逻辑一致）
    if (Norm.Y <= 0.0f)
    {
        SinVal = -SinVal;
    }

    // 3.4 计算半径并返回最终位置
    float Radius = ViewportSize.Y * ScreenRadiusPercent;
    FVector2D EdgePos;
    EdgePos.X = HalfViewport.X + Radius * CosVal;
    EdgePos.Y = HalfViewport.Y + Radius * SinVal;

    // 边界裁剪（安全防护）
    EdgePos.X = FMath::Clamp(EdgePos.X, 0.0f, ViewportSize.X);
    EdgePos.Y = FMath::Clamp(EdgePos.Y, 0.0f, ViewportSize.Y);

    return EdgePos;
}
