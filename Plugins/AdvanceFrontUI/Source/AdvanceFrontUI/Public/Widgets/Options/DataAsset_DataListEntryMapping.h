// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_DataListEntryMapping.generated.h"

class UListDataObject_Base;
class UWidget_ListEntry_Base;

/**
 * 数据列表条目映射资产
 * 数据资产，用于配置 UListDataObject_Base 子类到 UWidget_ListEntry_Base 子类的映射关系。
 * UFrontendUICommonListView 生成条目时会通过此类查找正确的条目控件类。
 * 支持继承链向上查找（如果找不到派生类的映射，则查找父类）。
 */
UCLASS()
class ADVANCEFRONTUI_API UDataAsset_DataListEntryMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * 根据数据对象查找对应的条目控件类
	 * 遍历数据对象的继承链，返回第一个匹配的条目控件类
	 */
	TSubclassOf<UWidget_ListEntry_Base> FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const;


private:
	/** 数据对象类 → 条目控件类的映射表（编辑器中配置） */
	UPROPERTY(EditDefaultsOnly, Category = "DataListEntryMapping")
	TMap<TSubclassOf<UListDataObject_Base>, TSubclassOf<UWidget_ListEntry_Base>> DataObjectListEntryMap;

};
