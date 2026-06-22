// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_OptionsScreen.generated.h"


class UOptionsDataRegistry;
class UFrontendTabListWidgetBase;
class UFrontendUICommonListView;
class UWidget_OptionsDetailView;
class UListDataObject_Base;
/**
 * 选项/设置界面控件
 * 包含选项卡导航、列表视图和详情视图三栏布局。
 * 通过 UOptionsDataRegistry 获取各选项卡的数据，支持重置为默认值的功能。
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	protected:

	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
    //End UUserWidget Interface


	//Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
    //end  UCommonActivatableWidget Interface

	private:

			/** 获取或懒创建选项数据注册表 */
		    UOptionsDataRegistry* GetOrCreateDataRegistry();

			//void UpdateDataRegistry();

			/** 语言切换时刷新所有数据对象的本地化文本（原地更新，不重建对象） */
			void RefreshLocalizedDataRegistry();

			/** 重置为默认值操作触发时的回调 */
		    void OnResetBoundActionTriggered();
			/** 返回操作触发时的回调 */
			void OnBackBoundActionTriggered();

			/** 当用户选择某个选项选项卡时调用 */
			UFUNCTION()
			void OnOptionsTabSelected(FName TabID);

			/** 列表项悬停状态改变时的回调 */
			void OnListViewItemHovered(UObject* InHoveredItem,bool bWasHovered);

			/** 列表项选中状态改变时的回调 */
			void OnListViewItemSelectionChanged(UObject* InSelectedItem);

			/** 尝试获取指定列表项对应的 Entry Widget 类名（调试用途） */
			FString TryGetEntryWidgetClassName(UObject* InOwningListItem)const;

			/** 列表数据被修改时的回调，用于维护可重置数据列表 */
			void OnListViewListDataModified(UListDataObject_Base* ModifiedData, EOptionsLsitDataModifyReason ModeifyReason);

			//Bound Widgets — 由蓝图绑定的子控件
			UPROPERTY(meta = (BindWidget))
			UFrontendTabListWidgetBase* TabListWidget_OptionsTabs;

			UPROPERTY(meta = (BindWidget))
			UFrontendUICommonListView* CommonListView_OptionsList;

			UPROPERTY(meta = (BindWidget))
			UWidget_OptionsDetailView* DetailView_ListEntryInfo;

			//BoundWidgets

			/** 懒创建的选项数据注册表 */
			UPROPERTY(Transient)
			UOptionsDataRegistry* CreatedOwningDataRegistry;

			/** 重置为默认值的输入操作数据表行引用 */
			UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
			FDataTableRowHandle ResetToDefaultAction;

			/** 重置操作的绑定句柄，用于动态添加/移除绑定 */
	        FUIActionBindingHandle ResetToDefaultActionHandle;

			/** 当前选项卡中所有可重置的列表数据对象 */
			UPROPERTY(Transient)
			TArray<UListDataObject_Base*> ResettableDataArray;

			/** 标记是否正在执行重置操作，防止重置过程中的重复回调 */
			bool bIsResettingData = false;

};
