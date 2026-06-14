// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	// 断言确保在项目设置中已配置加载画面控件蓝图
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Forgot to assign a valid widget blueprint in the project setting as loading screen"));

	// 同步加载控件类
	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget =  SoftLoadingScreenWidgetClass.LoadSynchronous();

	return LoadedLoadingScreenWidget;
}

