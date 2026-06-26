// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_CreatCharacter.generated.h"


class UCreatCharacterScreenDataRegistry;
class UFrontendTabListWidgetBase;
class UFrontendUICommonListView;
class UWidget_OptionsDetailView;
class UListDataObject_Base;

/**
 * 角色创建界面控件
 * 和 UWidget_OptionsScreen 结构类似：选项卡 + 列表视图 + 详情视图三栏布局。
 * 额外注册"确认"操作绑定（使用 CommonUI 默认确认操作），用于确定角色并保存数据。
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_CreatCharacter : public UWidget_ActivatableBase
{
    GENERATED_BODY()

protected:
    // Begin UUserWidget Interface
    virtual void NativeOnInitialized() override;
    // End UUserWidget Interface

    // Begin UCommonActivatableWidget Interface
    virtual void NativeOnActivated() override;
    virtual void NativeOnDeactivated() override;
    virtual UWidget* NativeGetDesiredFocusTarget() const override;
    // End UCommonActivatableWidget Interface

private:
    /** 获取或懒创建选项数据注册表 */
    UCreatCharacterScreenDataRegistry* GetOrCreateDataRegistry();

    /** 语言切换时刷新所有数据对象的本地化文本 */
    void RefreshLocalizedDataRegistry();

    /** 重置操作触发时的回调 */
    void OnResetBoundActionTriggered();
    /** 返回操作触发时的回调 */
    void OnBackBoundActionTriggered();
    /** 确认操作触发时的回调（角色创建完成后保存并进入游戏） */
    void OnConfirmCharacterActionTriggered();

    /** 当用户选择某个选项卡时调用 */
    UFUNCTION()
    void OnOptionsTabSelected(FName TabID);

    /** 列表项悬停状态改变时的回调 */
    void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);

    /** 列表项选中状态改变时的回调 */
    void OnListViewItemSelectionChanged(UObject* InSelectedItem);

    /** 尝试获取指定列表项对应的 Entry Widget 类名（调试用途） */
    FString TryGetEntryWidgetClassName(UObject* InOwningListItem) const;

    /** 列表数据被修改时的回调，用于维护可重置数据列表 */
    void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsLsitDataModifyReason ModifyReason);

    // ========== BoundWidgets ==========
    UPROPERTY(meta = (BindWidget))
    UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;

    UPROPERTY(meta = (BindWidget))
    UFrontendUICommonListView* CommonListView_OptionsList;

    UPROPERTY(meta = (BindWidget))
    UWidget_OptionsDetailView* DetailView_ListEntryInfo;

    // ========== 懒创建的数据注册表 ==========
    UPROPERTY(Transient)
    UCreatCharacterScreenDataRegistry* CreatedOwningDataRegistry;

    // ========== 操作绑定 ==========
    /** 重置为默认值的输入操作（DataTable 引用，蓝图中配置） */
    UPROPERTY(EditDefaultsOnly, Category = "Character Creation", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
    FDataTableRowHandle ResetToDefaultAction;

    /** 重置操作的绑定句柄 */
    FUIActionBindingHandle ResetToDefaultActionHandle;

    /** 确认操作的绑定句柄 */
    FUIActionBindingHandle ConfirmCharacterActionHandle;

    // ========== 数据状态 ==========
    /** 当前选项卡中所有可重置的列表数据对象 */
    UPROPERTY(Transient)
    TArray<UListDataObject_Base*> ResettableDataArray;

    /** 标记是否正在执行重置操作，防止重置过程中的重复回调 */
    bool bIsResettingData = false;
};
