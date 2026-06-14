// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

/**
 * 前端加载画面设置（开发者设置）
 * 在项目设置中配置加载画面控件蓝图类、额外保持时长、编辑器显示行为。
 */
UCLASS(Config = Game,DefaultConfig)
class ADVANCEFRONTUI_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** 获取加载画面控件类（同步加载，如果为空则断言） */
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;

	/** 加载画面控件蓝图类的软引用 */
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Setting")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;

	/** 加载完成后额外保持加载画面显示的秒数（防止闪烁） */
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Setting")
	float HoldLoadingScreenExtraSconds = 3.f;

	/** 是否在编辑器中显示加载画面（通常编辑器不需要） */
	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Setting")
	bool bShouldLoadingScreenInEditor = false;

};
