// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/MyListDataObject_Value.h"

void UMyListDataObject_Value::SetDataDynamicGetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicGetter)
{
	DataDynamicGetter = InDynamicGetter;  // 设置用于读取游戏设置值的动态 getter
}

void UMyListDataObject_Value::SetDataDynamicSetter(const TSharedPtr<FOptionsDataInteractionHelper>& InDynamicSetter)
{
	DataDynamicSetter = InDynamicSetter;  // 设置用于写入游戏设置值的动态 setter
}
