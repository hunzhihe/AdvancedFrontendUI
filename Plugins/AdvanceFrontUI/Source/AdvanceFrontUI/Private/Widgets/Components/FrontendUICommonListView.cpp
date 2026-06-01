// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendUICommonListView.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Options/DataAsset_DataListEntryMapping.h"
#include "Widgets/Options/ListEntrys/Widget_ListEntry_Base.h"
#include "Widgets/Options/ListDataObject_Base.h"



UUserWidget& UFrontendUICommonListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	// TODO: 在此处插入 return 语句
	if (IsDesignTime())
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}


	if (TSubclassOf<UWidget_ListEntry_Base> FoundWidgetClass = DataListEntryMapping->FindEntryWidgetClassByDataObject(Cast<UListDataObject_Base>(Item)))
	{
		return GenerateTypedEntry<UWidget_ListEntry_Base>(FoundWidgetClass, OwnerTable);
	}
	else
	{
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}
}

#if WITH_EDITOR
void UFrontendUICommonListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (!DataListEntryMapping)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable DataListEntryMapping is not assigned.")+
		GetClass()->GetName()+
			TEXT(" needs to have its DataListEntryMapping variable assigned.")
		));
	}

}
#endif