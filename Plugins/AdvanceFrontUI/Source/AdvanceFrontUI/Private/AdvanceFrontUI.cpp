// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvanceFrontUI.h"

#define LOCTEXT_NAMESPACE "FAdvanceFrontUIModule"

void FAdvanceFrontUIModule::StartupModule()
{
	// 模块加载后执行——加载时机由 .uplugin 文件中的 LoadingPhase 决定
}

void FAdvanceFrontUIModule::ShutdownModule()
{
	// 模块关闭时调用，用于清理。支持动态重载的模块会在卸载前调用此函数
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAdvanceFrontUIModule, AdvanceFrontUI)