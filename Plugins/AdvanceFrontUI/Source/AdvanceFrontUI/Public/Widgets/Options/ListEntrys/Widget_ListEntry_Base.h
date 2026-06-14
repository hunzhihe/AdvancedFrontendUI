// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_ListEntry_Base.generated.h"


class UCommonTextBlock;
class UListDataObject_Base;
/**
 * 列表条目控件基类
 * 所有选项列表条目的公共父类，实现 IUserObjectListEntry 接口。
 * 处理悬停高亮、选中状态切换、可编辑状态控制、数据对象绑定等核心逻辑。
 * 子类蓝图需实现具体的交互控件（滑块、旋转器等）。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	/** 蓝图实现事件：条目被悬停或取消悬停时调用 */
	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered,bool bIsEntryWidgetStillSelected);
	/** 原生悬停处理，触发蓝图事件并更新高亮状态 */
	void NativeOnListEntryWidgetHovered(bool bWasHovered);

protected:
	/** 蓝图实现事件：当手柄导航时返回需要聚焦的子控件 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "GetWidgetToFocusForGamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;

	/** 蓝图实现事件：切换条目的高亮状态 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnToggleEntryWidgetHighlightState"))
	void BP_OnToggleEntryWidgetHighlightState(bool bShouldHighlight) const;

	//Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual void NativeOnEntryReleased() override;
	//End IUserObjectListEntry Interface

	//Begin UUserWidget Interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//End UUserWidget Interface

	/** 当列表数据对象设置到此条目时调用（子类可重写以初始化具体的 UI 控件） */
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject);

	/** 当关联的列表数据对象被修改时调用 */
	virtual void OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason  ModifyReason);

	/** 当依赖的数据对象被修改时调用，用于刷新可编辑状态 */
	virtual void OnOwningDependencyDataObjectModified(UListDataObject_Base* OwningModifiedDependencyData, EOptionsLsitDataModifyReason  ModifyReason);

	/** 切换可编辑状态（启用/禁用交互控件） */
	virtual void OnToggleEditableState(bool bIsEditable);

	/** 在 ListView 中选中此条目 */
	void SelectThisListEntryWidget();
private:
	//Bound Widgets — 由蓝图绑定的设置名称文本
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional,AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//Bound Widgets

	/** 缓存的所属数据对象 */
	UPROPERTY(Transient)
	UListDataObject_Base* CachedOwningDataObject;


};
