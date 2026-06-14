// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontendStructTypes.generated.h"

/**
 * 选项数据编辑条件描述符
 * 封装了数据对象是否可编辑的判断逻辑、禁用原因、以及在禁用时是否强制设置特定值。
 * 用于 UListDataObject_Base 的编辑条件系统，支持多个条件叠加。
 */

USTRUCT()
struct FOptinsDataEditConditionDescriptor
{
	GENERATED_BODY()

public:
	/** 设置编辑条件判断函数（返回 true 表示该条件通过） */
	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc)
	{
		EditConditionFunc = InEditConditionFunc;
	}

	/** 判断此条件描述符是否已绑定有效的判断函数 */
	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	/** 判断当前编辑条件是否满足 */
	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}

		return false;

	}
	/** 获取禁用原因的富文本描述 */
	FString GetDisabledRichReason() const { return DisabledRichReason; }

	void SetDisabledRichReason(const FString& InDisabledRichReason)
	{
		DisabledRichReason = InDisabledRichReason;
	}

	/** 是否设置了强制值（当条件不满足时，强制将数据设置为某个特定值） */
	bool HasForcedStringVALUE() const
	{
		return DisabledForcedStringValue.IsSet();
	}
	/** 获取强制值字符串 */
	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& InForcedValue)
	{
		DisabledForcedStringValue = InForcedValue;
	}


private:
	/** 编辑条件判断函数（C++ Lambda，不序列化） */
	TFunction<bool()> EditConditionFunc;
	/** 条件不满足时的禁用原因富文本 */
	FString DisabledRichReason;
	/** 条件不满足时的强制值（可选，未设置则不强制） */
	TOptional<FString> DisabledForcedStringValue;

};
