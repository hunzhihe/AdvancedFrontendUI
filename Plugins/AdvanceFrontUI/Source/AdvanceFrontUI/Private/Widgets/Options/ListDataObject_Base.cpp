// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListDataObject_Base.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

void UListDataObject_Base::InitDataObject()
{
	// 调用子类可重写的初始化回调
	OnDataObjectInitialized();
}

void UListDataObject_Base::AddEditCondition(const FOptinsDataEditConditionDescriptor& InEditCondition)
{
	EditConditionDescArray.Add(InEditCondition);
}

void UListDataObject_Base::AddEditDependencyData(UListDataObject_Base* InDependencyData)
{
	// 如果尚未绑定，绑定依赖数据的修改事件
	if (!InDependencyData->OnListDataModified.IsBoundToObject(this))
	{
		InDependencyData->OnListDataModified.AddUObject(
			this,
			&ThisClass:: OnEditDependencyDataModifiied
			);
	}
}

bool UListDataObject_Base::IsDataCurrentlyEditable()
{
	bool bIsEditable = true;

	// 无编辑条件则默认可编辑
	if (EditConditionDescArray.IsEmpty())
	{
		return bIsEditable;
	}

	FString CachedDisabledRichReason;

	// 遍历所有编辑条件，任一不满足则标记为不可编辑
	for (const FOptinsDataEditConditionDescriptor& Condition : EditConditionDescArray)
	{
		if (!Condition.IsValid() || Condition.IsEditConditionMet())
		{
			continue;
		}

		bIsEditable = false;

		// 收集所有禁用原因
		CachedDisabledRichReason.Append(Condition.GetDisabledRichReason());

		SetDisabledRichText(FText::FromString(CachedDisabledRichReason));

		// 如果条件指定了强制值，则尝试设置
		if (Condition.HasForcedStringVALUE())
		{
			const FString ForcedStringValue = Condition.GetDisabledForcedStringValue();

			if (CanSetToForcedStringvalue(ForcedStringValue))
			{
				OnSetToForcedStringvalue(ForcedStringValue);
			}

		}
	}
	return bIsEditable;

}

void UListDataObject_Base::OnDataObjectInitialized()
{
}

void UListDataObject_Base::NotifyListDataModified(UListDataObject_Base* InModifiedListDataObject, EOptionsLsitDataModifyReason ModifyReason)
{
	// 广播修改事件给所有监听者（如对应的 ListEntry Widget）
	OnListDataModified.Broadcast(InModifiedListDataObject, ModifyReason);

	// 如果标记为立即应用，则同步保存到游戏用户设置
	if (bShouldApplyChangeImmediatly)
	{
		UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->ApplySettings(true);
	}
}

void UListDataObject_Base::OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason)
{
	// 广播依赖数据修改事件（如 WindowMode 变更影响 ScreenResolution 的可编辑性）
	OnDependencyDataModified.Broadcast(InModifiedDependencyData, ModifyReason);
}
