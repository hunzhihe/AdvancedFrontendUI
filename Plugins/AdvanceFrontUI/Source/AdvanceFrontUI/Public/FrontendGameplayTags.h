// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"


/**
 * 前端 GameplayTag 命名空间
 * 集中声明所有前端 UI 系统使用的原生 GameplayTag。
 * 分为三大类：控件堆栈标签、控件类型标签、图片资产标签。
 */
namespace FrontendGameplayTags
{
	// ========== 控件堆栈标签（Frontend.WidgetStack.*）==========
	// 用于标识 UWidget_PrimaryLayout 中注册的不同控件堆栈容器
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);       // 模态堆栈（弹窗、确认框）
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);    // 游戏菜单堆栈
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHud);     // 游戏 HUD 堆栈
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);    // 主前端界面堆栈

	// ========== 控件类型标签（Frontend.Widget.*）==========
	// 用于在前端设置中映射标签到具体的控件蓝图类
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_StoryScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_OptionsScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_KeyRemapScreen);

	// ========== 选项图片标签（Frontend.Image.*）==========
	// 用于在前端设置中映射标签到具体的图片纹理资产
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Image_TestIamge);

}