// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "FrontendUIFunctionLibrary.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"

void UListDataObject_String::OnDataObjectInitialized()
{
	// 默认选中第一个选项
	if (!AvaiableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvaiableOptionsStringArray[0];
	}

	// 如果有默认值，覆盖当前选中
	if (HasDefaultValue())
	{
		CurrentStringValue = GetDefualtValueAsString();
	}

	// 从 getter 读取当前实际值（优先级最高）
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	// 根据字符串值查找对应的显示文本
	if (!TrySetDisplayTextByStringValue(CurrentStringValue))
	{
			CurrentDisplayText = FText::FromString("Invalid Options");		
	};
}

void UListDataObject_String::AddDynamicOption(const FString& OptionString, const FText& OptionDisplayText)
{
	// FrameRateLimit、ScreenResolution 等系统选项的值不需要本地化，直接使用传入的显示文本
	if (GetDataID() == "FrameRateLimit" || GetDataID() == "ScreenResolution")
	{
		AvaiableOptionsStringArray.Add(OptionString);
		AvaiableOptionsDisplayTextArray.Add(OptionDisplayText);
		AvaiableOptionsLocalizationKeyArray.Add(TEXT(""));
		return;
	}


	if (UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetCurrentLanguage() == ELaughageChanged::English)
	{
		AvaiableOptionsStringArray.Add(OptionString);
		AvaiableOptionsDisplayTextArray.Add(UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::English, OptionString));
		AvaiableOptionsLocalizationKeyArray.Add(TEXT(""));  // 空键 = 使用 OptionString 作为查表键
	}
	else if (UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetCurrentLanguage() == ELaughageChanged::ZH_Ch)
	{
		AvaiableOptionsStringArray.Add(OptionString);
		AvaiableOptionsDisplayTextArray.Add(UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(ELaughageChanged::ZH_Ch, OptionString));
		AvaiableOptionsLocalizationKeyArray.Add(TEXT(""));
	}

}

void UListDataObject_String::RefreshLocalizedText()
{
	// 先调用基类刷新描述文本
	Super::RefreshLocalizedText();

	// FrameRateLimit 的系统选项值不需要本地化
	if (GetDataID() == FName("FrameRateLimit"))
	{
		return;
	}

	// 清空并重新填充显示文本数组（用当前语言从 StringTable 查表）
	const ELaughageChanged CurrentLanguage = UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetCurrentLanguage();
	AvaiableOptionsDisplayTextArray.Empty();

	// 确保本地化键数组与选项数组大小一致
	if (AvaiableOptionsLocalizationKeyArray.Num() != AvaiableOptionsStringArray.Num())
	{
		AvaiableOptionsLocalizationKeyArray.SetNum(AvaiableOptionsStringArray.Num());
	}

	for (int32 i = 0; i < AvaiableOptionsStringArray.Num(); ++i)
	{
		// 优先使用自定义本地化键，为空时回退到 OptionString
		const FString& LookupKey = AvaiableOptionsLocalizationKeyArray.IsValidIndex(i) && !AvaiableOptionsLocalizationKeyArray[i].IsEmpty()
			? AvaiableOptionsLocalizationKeyArray[i]
			: AvaiableOptionsStringArray[i];

		AvaiableOptionsDisplayTextArray.Add(
			UFrontendUIFunctionLibrary::GetCurrentLanguageTextFromTable(CurrentLanguage, LookupKey)
		);
	}

	// 刷新当前选中值的显示文本
	TrySetDisplayTextByStringValue(CurrentStringValue);
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (AvaiableOptionsStringArray.IsEmpty() || AvaiableOptionsDisplayTextArray.IsEmpty())
	{
		return;
	}

	const int32 CurrentDisplayIndex = AvaiableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndex = (CurrentDisplayIndex + 1) % AvaiableOptionsStringArray.Num();

	const bool bIsNaxtIndexValid = AvaiableOptionsStringArray.IsValidIndex(NextIndex);

	if (bIsNaxtIndexValid)
	{
		CurrentStringValue = AvaiableOptionsStringArray[NextIndex];	
	}
	else
	{
		CurrentStringValue = AvaiableOptionsStringArray[0];	
	}
	TrySetDisplayTextByStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}	
}

void UListDataObject_String::BackToPerviousOption()
{
	if (AvaiableOptionsStringArray.IsEmpty() || AvaiableOptionsDisplayTextArray.IsEmpty())
	{
		return;
	}
	const int32 CurrentDisplayIndex = AvaiableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PreviousIndex = (CurrentDisplayIndex - 1 + AvaiableOptionsStringArray.Num()) % AvaiableOptionsStringArray.Num();
	const bool bIsPreviousIndexValid = AvaiableOptionsStringArray.IsValidIndex(PreviousIndex);
	if (bIsPreviousIndexValid)
	{
		CurrentStringValue = AvaiableOptionsStringArray[PreviousIndex];
	}
	else
	{
		CurrentStringValue = AvaiableOptionsStringArray.Last();
	}
	TrySetDisplayTextByStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::OnRotatorInitiatedValueChange(const FText& InNewSelectedText)
{
	const int32 FoundIndex = AvaiableOptionsDisplayTextArray.IndexOfByPredicate(
		[InNewSelectedText](const FText& AvailableText)->bool 
		{
			return AvailableText.EqualTo(InNewSelectedText);
		}
	);

	if (FoundIndex != INDEX_NONE && AvaiableOptionsStringArray.IsValidIndex(FoundIndex))
	{
		CurrentDisplayText = InNewSelectedText;
		CurrentStringValue = AvaiableOptionsStringArray[FoundIndex];

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);
			NotifyListDataModified(this);
		}
	}
}



bool UListDataObject_String::CanResetBackToDefaultValue() const
{
	return HasDefaultValue() && CurrentStringValue != GetDefualtValueAsString();
}

bool UListDataObject_String::TryResetBackToDefaultValue() 
{
	if (CanResetBackToDefaultValue())
	{
		CurrentStringValue = GetDefualtValueAsString();
		TrySetDisplayTextByStringValue(CurrentStringValue);

		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(CurrentStringValue);

			NotifyListDataModified(this, EOptionsLsitDataModifyReason::ResetToDefault);

			return true;

		}
	}
	return false;
}

bool UListDataObject_String::CanSetToForcedStringvalue(const FString& InForcedValue) const
{
	return CurrentStringValue != InForcedValue;
}

void UListDataObject_String::OnSetToForcedStringvalue(const FString& InForcedValue)
{
	CurrentStringValue = InForcedValue;
	TrySetDisplayTextByStringValue(CurrentStringValue);

	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueFromString(CurrentStringValue);
		NotifyListDataModified(this, EOptionsLsitDataModifyReason::DependencyModified);
	}
}

bool UListDataObject_String::TrySetDisplayTextByStringValue(const FString& InStringValue)
{
	const int32 Index = AvaiableOptionsStringArray.IndexOfByKey(InStringValue);
	if (Index != INDEX_NONE)
	{
		//CurrentStringValue = InStringValue;
		CurrentDisplayText = AvaiableOptionsDisplayTextArray[Index];
		return true;
	}

	return false;
}




void UListDataObject_String::SetOptionLocalizationKey(const FString& OptionString, const FString& InKey)
{
	const int32 Index = AvaiableOptionsStringArray.IndexOfByKey(OptionString);
	if (Index != INDEX_NONE)
	{
		// 确保数组大小一致
		if (AvaiableOptionsLocalizationKeyArray.Num() != AvaiableOptionsStringArray.Num())
		{
			AvaiableOptionsLocalizationKeyArray.SetNum(AvaiableOptionsStringArray.Num());
		}
		AvaiableOptionsLocalizationKeyArray[Index] = InKey;
	}
}

//******** UListDataObject_StringInteger ********//
void UListDataObject_StringInteger::AddIntegerOption(int32 InIntegerValue, const FText& InDispalyText)
{
	AddDynamicOption(LexToString(InIntegerValue), InDispalyText);

}

void UListDataObject_StringInteger::SetLocalizationKeyForIntegerValue(int32 InIntegerValue, const FString& InKey)
{
	SetOptionLocalizationKey(LexToString(InIntegerValue), InKey);
}

void UListDataObject_StringInteger::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	// 如果当前值不在预设列表中则显示 "Custom"（如 OverallQuality 被其他设置间接修改）
	if (!TrySetDisplayTextByStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Custom"));
	}
}

void UListDataObject_StringInteger::OnEditDependencyDataModifiied(UListDataObject_Base* InModifiedDependencyData, EOptionsLsitDataModifyReason ModifyReason)
{
	if (DataDynamicGetter)
	{
		// 如果值未变化则无需更新
		if (CurrentStringValue == DataDynamicGetter->GetValueAsString())
		{
			return;
		}

		// 从 getter 同步最新值
		CurrentStringValue = DataDynamicGetter->GetValueAsString();

		if (!TrySetDisplayTextByStringValue(CurrentStringValue))
		{
			CurrentDisplayText = FText::FromString(TEXT("Custom"));
		}

		NotifyListDataModified(this, EOptionsLsitDataModifyReason::DependencyModified);
	}
	Super::OnEditDependencyDataModifiied(InModifiedDependencyData, ModifyReason);
}
