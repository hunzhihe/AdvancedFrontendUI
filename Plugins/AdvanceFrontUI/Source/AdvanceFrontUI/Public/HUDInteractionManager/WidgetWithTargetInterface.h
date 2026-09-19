// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetWithTargetInterface.generated.h"


class UManager_InteractionTarget;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWidgetWithTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEFRONTUI_API IWidgetWithTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    // 获取关联的 InteractionTarget
    // 蓝图中实现
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    UManager_InteractionTarget* GetInteractionTarget() const;

    // 设置关联的 InteractionTarget（可选）
    // 蓝图中实现
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
    void SetInteractionTarget(UManager_InteractionTarget* NewTarget);
};
