// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntrys/Widget_ListEntry_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Components/FrontendCommonRotator.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "CommonInputSubsystem.h"
#include "Internationalization/StringTableRegistry.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"
#include "Subsystem/FrontendUISubsystem.h"

#include "FrontendUIDebugHelper.h"


#define GET_Zh_DESCRIPTION(InKey)LOCTABLE("/Game/Blueprints/UI/StringTable/ST_ZH_OptionScreenDescription.ST_ZH_OptionScreenDescription", InKey)

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 绑定前后翻页按钮点击事件
	CommonButton_PreviousOption->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &UWidget_ListEntry_String::OnNextOptionButtonClicked);

	// 点击 Rotator 时选中此列表条目
	CommonRotator_AvailableOptions->OnClicked().AddLambda([this]()
		{
			SelectThisListEntryWidget();
		});

	// 绑定 Rotator 旋转事件
	CommonRotator_AvailableOptions->OnRotatedEvent.AddUObject(this, &ThisClass::OnRotatorValueChanged);

}

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InListDataObject)
{
	Super::OnOwningListDataObjectSet(InListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvaiableOptionsDisplayTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_String::OnOwingListDataObjectModifed(UListDataObject_Base* OwningModifiedData, EOptionsLsitDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}

}

void UWidget_ListEntry_String::OnToggleEditableState(bool bIsEditable)
{
	Super::OnToggleEditableState(bIsEditable);


	CommonButton_PreviousOption->SetIsEnabled(bIsEditable);
	CommonRotator_AvailableOptions->SetIsEnabled(bIsEditable);
	CommonButton_NextOption->SetIsEnabled(bIsEditable);
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPerviousOption();

		// 语言切换时触发本地化文本刷新
		if (CachedOwningStringDataObject->GetDataID() == FName("Language"))
		{
			if (UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this))
			{
				FrontendUISubsystem->OnRegistryNewOptionsData.ExecuteIfBound();

				// 测试：打印刷新后的显示文本数组
				const TArray<FText>& DisplayTextArray = CachedOwningStringDataObject->GetAvaiableOptionsDisplayTextArray();
				for (int32 i = 0; i < DisplayTextArray.Num(); ++i)
				{
					FrontendUIDebugHelper::Log(FString::Printf(TEXT("DisplayText[%d]: %s"), i, *DisplayTextArray[i].ToString()));
				}
			}
		}
	}
	SelectThisListEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();

		// 语言切换时触发本地化文本刷新
		if (CachedOwningStringDataObject->GetDataID() == FName("Language"))
		{
			if (UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this))
			{
				FrontendUISubsystem->OnRegistryNewOptionsData.ExecuteIfBound();

				CachedOwningStringDataObject->GetCurrentDisplayText();
			}
		}
	}
	SelectThisListEntryWidget();
}

void UWidget_ListEntry_String::OnRotatorValueChanged(int32 Value, bool bUserInitiated)
{
	if (!CachedOwningStringDataObject)
	{
		return;
	}
	UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem();

	// 仅处理用户主动操作且为手柄输入的情况
	if (!CommonInputSubsystem || !bUserInitiated)
	{
		return;
	}

	if (CommonInputSubsystem->GetCurrentInputType() == ECommonInputType::Gamepad)
	{
		CachedOwningStringDataObject->OnRotatorInitiatedValueChange(CommonRotator_AvailableOptions->GetSelectedText());

		// 语言切换时触发本地化文本刷新
		if (CachedOwningStringDataObject->GetDataID() == FName("Language"))
		{
			if (UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(this))
			{
				FrontendUISubsystem->OnRegistryNewOptionsData.ExecuteIfBound();
			}
		}
	}
}
