// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

/**
 * 集合类型的列表数据对象
 * 用于组织树形数据结构（如选项卡 > 分类 > 具体设置项）。
 * 持有子级 UListDataObject_Base 数组，代表一个可展开的数据组。
 */
UCLASS()
class ADVANCEFRONTUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()

public:

	/** 添加子级列表数据，自动调用 InitDataObject 并设置 ParentData */
	void AddChildListData(UListDataObject_Base* InChildListData);

	/** 获取所有子级列表数据 */
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const;

	/** 判断是否有子级列表数据 */
	virtual bool HasAnyChildListData() const;

private:

	/** 子级列表数据数组（Transient，运行时持有） */
	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;

};
