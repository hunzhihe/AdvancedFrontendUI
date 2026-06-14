// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendUICommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widgets/Options/ListDataObject_Base.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"



UUserWidget& UFrontendUICommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	// 设计时使用默认生成逻辑
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}

	// 运行时通过 DataListEntryMapping 查找匹配的条目控件类
	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(Cast<UListDataObject_Base>(Item)))
	{
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}

bool UFrontendUICommonListView::OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem)
{
	// ListDataObject_Collection 是分组容器，不可选中
	return !FirstSelectedItem->IsA<UListDataObject_Collection>();
}

#if WITH_EDITOR
void UFrontendUICommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	// 编译时验证：DataListEntryMapping 必须赋值
	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable DataListEntryMapping is not assigned.")+
		GetClass()->GetName()+
			TEXT(" needs to have its DataListEntryMapping variable assigned.")
		));
	}

}
#endif