// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"


namespace FrontendGameplayTags
{
	// Example Gameplay Tag
	// You can add your own tags here and use them in Blueprints or C++ code.
	// For example: GameplayTag MyTag = FGameplayTag::RequestGameplayTag("MyGame.MyTag");

	// Widget Stack Gameplay Tags
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHud);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);

	//Frontend Widgets
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_StoryScreen);
	ADVANCEFRONTUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_OptionsScreen);
}