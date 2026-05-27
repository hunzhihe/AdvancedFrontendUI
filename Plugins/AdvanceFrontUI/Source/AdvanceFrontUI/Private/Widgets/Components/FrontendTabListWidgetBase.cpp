// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/FrontendTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"

#if WITH_EDITOR
void UFrontendTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog &CompileLog) const
{
    Super::ValidateCompiledDefaults(CompileLog);
    if (!TabButtonEntryWidgetClass)
    {
        CompileLog.Error(FText::FromString(
            TEXT("变量 TabButtonEntryWidgetClass 没有指定有效的条目，") +
            GetClass()->GetName() +
            TEXT(" 需要一个有效的条目控件类才能正常工作")
        ));
    }
}
#endif