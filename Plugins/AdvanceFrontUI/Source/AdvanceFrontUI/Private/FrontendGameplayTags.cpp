// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontendGameplayTags.h"

namespace FrontendGameplayTags
{
	// ========== 控件堆栈标签定义 ==========
    UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHud, "Frontend.WidgetStack.GameHud");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");

	// ========== 控件类型标签定义 ==========
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_StoryScreen, "Frontend.Widget.StoryScreen");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_OptionsScreen, "Frontend.Widget.OptionsScreen");
    UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_KeyRemapScreen, "Frontend.Widget.KeyRemapScreen");

    // ========== 图片资产标签定义 ==========
    UE_DEFINE_GAMEPLAY_TAG(Frontend_Image_TestIamge, "Frontend.Image.TestIamge");
}