// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FrontendSettings/FrontendUIGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

#include "FrontendUIDebugHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	// 获取系统支持的所有全屏分辨率
	TArray<FIntPoint> AvaialbeResolutionss;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvaialbeResolutionss);

	// 按分辨率从小到大排序
	AvaialbeResolutionss.Sort(
		[](const FIntPoint& A, const FIntPoint& B)->bool {

			return A.SizeSquared() < B.SizeSquared();
		}

	);

	// 将所有分辨率添加为选项
	for (const FIntPoint& Resolution: AvaialbeResolutionss)
	{
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	// 记录最大分辨率（用于非全屏窗口模式的强制值）
	MaximumAllowedResolution = ResToValueString( AvaialbeResolutionss.Last());

	// 默认选中最大分辨率
	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::RefreshLocalizedText()
{
	// 只刷新描述和显示名称（基类逻辑），跳过选项值文本本地化
	// 分辨率字符串如 "(1920 x 1080)" 是技术值，不需要本地化
	UListDataObject_Base::RefreshLocalizedText();
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{

	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextByStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UFrontendUIGameUserSettings::GetFrontendUIGameUserSettings()->GetScreenResolution());
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	return FString::Printf(TEXT("(X=%i,Y=%i)"),InResolution.X,InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString =   FString::Printf(TEXT("(%i x %i)"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
