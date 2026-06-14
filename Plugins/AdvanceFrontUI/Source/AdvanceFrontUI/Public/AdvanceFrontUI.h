// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

/**
 * AdvanceFrontUI 插件模块主类
 * 负责插件的初始化和清理工作，是 UE 模块系统的入口
 */
class FAdvanceFrontUIModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
