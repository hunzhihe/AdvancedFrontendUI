// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "FrontendTypes/FrontendEnumType.h"
#include "Widget_ConformScreen.generated.h"


class UCommonTextBlock;
class UDynamicEntryBox;

/**
 * 确认画面按钮信息结构体
 * 描述确认画面中单个按钮的类型和显示文本
 */
USTRUCT(BlueprintType)
struct FConformScreenButtonInfo
{
	GENERATED_BODY()

	/** 此按钮对应的确认结果类型（确认/取消/关闭） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

	/** 按钮上显示的文本 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;


};
/**
 * 确认画面信息对象
 * 封装确认画面的标题、消息和可用按钮列表。
 * 提供静态工厂方法创建不同类型的确认画面（OK、Yes/No、OK/Cancel）。
 */
UCLASS()
class ADVANCEFRONTUI_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()
	public:

	/** 创建仅包含 OK 按钮的确认画面 */
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	/** 创建包含 Yes/No 按钮的确认画面 */
    static UConfirmScreenInfoObject* CreateYesNoScreen(const FText& InScreenTitle, const FText& InScreenMessage);
	/** 创建包含 OK/Cancel 按钮的确认画面 */
    static UConfirmScreenInfoObject* CreateOKCancelScreen(const FText& InScreenTitle, const FText& InScreenMessage);

	UPROPERTY(Transient)
	FText ScreenTitle;

	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	TArray<FConformScreenButtonInfo> AvailableDcreenButtons;

};



/**
 * 确认画面控件
 * 模态对话框风格的确认界面，支持 OK、Yes/No、OK/Cancel 三种模式。
 * 按钮点击后自动停用自身并通过回调返回结果。
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ADVANCEFRONTUI_API UWidget_ConformScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()

	public:
	/**
	 * 初始化确认画面
	 * @param InConfirmScreenInfoObject 画面配置信息（标题、消息、按钮）
	 * @param InButtonClickedCallback   按钮点击回调，参数为点击的按钮类型
	 */
	void InitConformScreen(
		UConfirmScreenInfoObject* InConfirmScreenInfoObject,TFunction<void(EConfirmScreenButtonType)> InButtonClickedCallback);


	protected:
	//Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//end  UCommonActivatableWidget Interface

	private:
	//Bound Widgets — 由蓝图绑定的子控件
	UPROPERTY(META = (BindWidget))
	UCommonTextBlock* CommonTextBlock_ConformScreenTitle;

	UPROPERTY(META = (BindWidget))
	UCommonTextBlock* CommonTextBlock_ConformScreenMessage;

	UPROPERTY(META = (BindWidget))
	UDynamicEntryBox* DynamicEntryBox_ConformScreenButton;
};
