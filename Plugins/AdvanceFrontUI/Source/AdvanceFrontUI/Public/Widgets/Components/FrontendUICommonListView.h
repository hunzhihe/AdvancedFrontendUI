// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "FrontendUICommonListView.generated.h"

class UDataAsset_DataListEntryMapping;

/**
 * 前端通用列表视图控件
 * 继承自 UCommonListView，通过 UDataAsset_DataListEntryMapping 自动选择
 * 列表数据对象对应的条目控件类。支持编辑器编译时验证。
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUICommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:

	//Begin UCommonListView Interface
	/** 生成条目控件时自动从 DataListEntryMapping 查找匹配的 Widget 类 */
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	/** ListDataObject_Collection 不可选中或导航 */
	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	//End UCommonListView Interface

private:
#if WITH_EDITOR
	/** 编译器验证：确保 DataListEntryMapping 不为空 */
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

	/** 列表数据对象到条目控件类的映射资产 */
	UPROPERTY(EditAnywhere,Category = "Frontend List View Settings")
	UDataAsset_DataListEntryMapping* DataListEntryMapping;

};
