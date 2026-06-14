// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"


void UListDataObject_Collection::AddChildListData(UListDataObject_Base* InChildListData)
{
	// 初始化子数据对象（调用 OnDataObjectInitialized）
	InChildListData->InitDataObject();

	// 设置父级引用，建立树形关系
	InChildListData->SetParentData(this);

	// 添加到子级数组
	ChildListDataArray.Add(InChildListData);
}

TArray<UListDataObject_Base*> UListDataObject_Collection::GetAllChildListData() const
{
	return ChildListDataArray;
}

bool UListDataObject_Collection::HasAnyChildListData() const
{
	return !ChildListDataArray.IsEmpty();
}
