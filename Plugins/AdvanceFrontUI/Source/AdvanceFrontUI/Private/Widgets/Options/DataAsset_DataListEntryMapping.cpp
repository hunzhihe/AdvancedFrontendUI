// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/ListDataObject_Base.h"


TSubclassOf<UWidget_ListEntry_Base> UDataAsset_DataListEntryMapping::FindEntryWidgetClassByDataObject(UListDataObject_Base* InDataObject) const
{
	check(InDataObject);
	// 遍历数据对象的继承链，从最派生类向上查找
	for (UClass* DataObjectClass =  InDataObject->GetClass(); DataObjectClass;DataObjectClass ->GetSuperClass() )
	{
		if (TSubclassOf<UListDataObject_Base> ConvertedDataObjectClass = TSubclassOf<UListDataObject_Base>(DataObjectClass))
		{
			if (DataObjectListEntryMap.Contains(ConvertedDataObjectClass))
			{
				return DataObjectListEntryMap.FindRef(ConvertedDataObjectClass);
			}
		}
	}

	// 未找到匹配的条目控件类
	return TSubclassOf<UWidget_ListEntry_Base>();
}
