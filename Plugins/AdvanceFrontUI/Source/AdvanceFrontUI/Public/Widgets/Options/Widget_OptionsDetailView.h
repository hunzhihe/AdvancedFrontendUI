// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_OptionsDetailView.generated.h"


class UCommonTextBlock;
class UCommonLazyImage;
class UCommonRichTextBlock;
class UListDataObject_Base;
/**
 * 选项详情视图控件
 * 显示选中列表项的详细信息：标题、描述图片、富文本描述、
 * 动态详情（类名信息）、禁用原因等。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNaiveTick))
class ADVANCEFRONTUI_API UWidget_OptionsDetailView : public UUserWidget
{
	GENERATED_BODY()


public:
	/** 根据列表数据对象更新详情视图的内容 */
	void UpdateDetailView(UListDataObject_Base* InDataObject,const FString& InEntryWidgetClassName = FString());
	/** 清空详情视图的所有内容 */
	void ClearDetailView();

protected:
	//Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//End UUserWidget Interface

private:
	//Bound Widgets — 由蓝图绑定的子控件
	UPROPERTY(meta =(BindWidget))
	UCommonTextBlock* TextBlock_Title;

	UPROPERTY(meta = (BindWidget))
	UCommonLazyImage* CommonLazyImage_DescriptionImage;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_Description;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DynamicDetails;

	UPROPERTY(meta = (BindWidget))
	UCommonRichTextBlock* CommonRichTextBlock_DisableReason;


	//Bound Widgtes

};
