// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntrys/Widget_ListEntry_KeyRemap.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Subsystem/FrontendUISubsystem.h"
#include "FrontendGameplayTags.h"
#include "FrontendUIFunctionLibrary.h"
#include "Widgets/Widget_KeyRemapScreen.h"


#include "FrontendUIDebugHelper.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 绑定重映射和重置按钮点击事件
	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);


}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject)
{
	Super::OnOwningListDataObjectSet(InListDataObject);

	CachedOwningKeyRemapDataObject= CastChecked<UListDataObject_KeyRemap>(InListDataObject);

	// 设置当前绑定按键的图标
	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason ModifyReason)
{
	// 数据修改后刷新按键图标
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisListEntryWidget();

	// 推送按键捕获画面到模态堆栈
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendUIFunctionLibrary::GetFrontendWidgetFromTag(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState,UWidget_ActivatableBase* PushedWidget) {

			// 在控件创建后、推入堆栈前配置按键捕获画面
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen =  CastChecked<UWidget_KeyRemapScreen>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPress);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(this, &ThisClass::OnKeyRemapCanceled);
				if (CreatedKeyRemapScreen)
				{
					// 根据目标输入类型过滤按键
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}


			}

		}


	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisListEntryWidget();

	if (!CachedOwningKeyRemapDataObject)
	{
		return;
	}

	// 检查当前按键是否已经是默认按键
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		// 已经是默认值，显示提示画面
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
			EConfirmScreenType::OK,
			FText::FromString(TEXT("Reset Key Mapping")),
			FText::FromString(TEXT("The Key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" is already set to Default")),
		    [](EConfirmScreenButtonType ClickedButton){}

			);
		return;
	}

	// 弹出确认对话框后执行重置
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),
		FText::FromString(TEXT("Are you sure you want to reset the key binding for ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT("?")),
		[this](EConfirmScreenButtonType ClickButton) {

			if (ClickButton == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}

		}

	);
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPress(const FKey& PressedKey)
{
	// 将捕获到的按键绑定到数据对象
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CanaceledReason)
{
	// 按键捕获取消时显示确认画面
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModelStackAsync(
		EConfirmScreenType::OK,
		FText::FromString(TEXT("Key Remp")),
		FText::FromString(CanaceledReason),
		[](EConfirmScreenButtonType ClickedButton) {}


	);
}
