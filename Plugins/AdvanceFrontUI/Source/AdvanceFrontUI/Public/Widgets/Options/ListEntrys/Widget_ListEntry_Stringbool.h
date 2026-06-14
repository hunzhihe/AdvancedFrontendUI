// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_String.h"
#include "Widget_ListEntry_Stringbool.generated.h"

/**
 * 布尔值列表条目控件
 * 继承自 UWidget_ListEntry_String，使用 Rotator 在 ON/OFF（或自定义显示文本）之间切换。
 * 实际的交互逻辑由父类 UWidget_ListEntry_String 处理，此类主要用于 DataAsset 映射中的类型区分。
 */
UCLASS()
class ADVANCEFRONTUI_API UWidget_ListEntry_Stringbool : public UWidget_ListEntry_String
{
	GENERATED_BODY()

};
