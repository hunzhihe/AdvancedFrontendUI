// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "FrontendTabListWidgetBase.generated.h"



/**
 * 前端选项卡列表控件基类
 * 继承自 UCommonTabListWidgetBase，使用 UFrontendCommonButtonBase 作为选项卡按钮。
 * 支持运行时动态注册选项卡。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UFrontendTabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	/** 动态注册一个选项卡（创建按钮并设置显示文本） */
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);

private:

#if WITH_EDITOR
	/** 编辑器编译验证：确保 TabButtonEntryWidgetClass 不为空 */
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif

	/** 编辑器预览时显示的调试选项卡数量 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendTabList", meta = (AllowPrivateAccess = "true",ClampMin = "1",ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;

	/** 选项卡按钮的条目控件类（必须派生自 UFrontendCommonButtonBase） */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FrontendTabList", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UFrontendCommonButtonBase> TabButtonEntryWidgetClass;

};
