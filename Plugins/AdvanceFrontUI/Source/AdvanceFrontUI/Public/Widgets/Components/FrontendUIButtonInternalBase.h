// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendUIButtonInternalBase.generated.h"

/**
 * 前端 UI 按钮内部基类
 * 继承自 UCommonButtonInternalBase，提供对内层 Slate 按钮控件的公共访问接口。
 * UFrontendCommonButtonBase 通过此类创建自定义内部按钮实例。
 */
UCLASS()
class ADVANCEFRONTUI_API UFrontendUIButtonInternalBase : public UCommonButtonInternalBase
{
	GENERATED_BODY()

public:
	/** 获取底层的 Slate 按钮控件 */
	TSharedPtr<class SCommonButton> GetSlateButton() const { return MyCommonButton; }

};
