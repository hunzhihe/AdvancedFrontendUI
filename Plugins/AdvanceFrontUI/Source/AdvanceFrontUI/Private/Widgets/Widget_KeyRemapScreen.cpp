// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Widget_KeyRemapScreen.h"
#include "CommonRichTextBlock.h"
#include "Framework/Application/IInputProcessor.h"
#include "CommonInputSubsystem.h"
#include "ICommonInputModule.h"
#include "CommonUITypes.h"

#include "FrontendUIDebugHelper.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo,ULocalPlayer* InOwingLocalPlayer )
		: CachedInputTypeToListenTo(InInputTypeToListenTo)
		,CachedWeakOwningLocalPlayer(InOwingLocalPlayer)
	{
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate,const FKey& /*PressKey*/)
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCanceledDelegate,const FString& /*CanceledReason*/)
	FOnInputPreProcessorKeySelectCanceledDelegate OnInputPreProcessorKeySelectedCanceled;


public:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{

	}
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{ 
		//FrontendUIDebugHelper::Log(TEXT("Press Key:") + InKeyEvent.GetKey().GetDisplayName().ToString());

		//UEnum* StaticCommonInputType = StaticEnum<ECommonInputType>();

		//FrontendUIDebugHelper::Log(TEXT("Desired input Key Type:") + StaticCommonInputType->GetValueAsString(CachedInputTypeToListenTo));
		ProcessPressedKey(InKeyEvent.GetKey());
		
		return true; 
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override 
	{ 
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		//FrontendUIDebugHelper::Log(TEXT("Press Key:") + MouseEvent.GetEffectingButton().GetDisplayName().ToString());
		return true; 
	}

	void ProcessPressedKey(const FKey& InPressKey)
	{
		if (InPressKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Key Remap has been canceled"));
		    
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem =  UCommonInputSubsystem::Get(CachedWeakOwningLocalPlayer.Get());

		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:

			if (InPressKey.IsGamepadKey()||CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Delected GamePad Key Pressed for keyboard inputs, Key Remap has been cancled."));
				return;
			}


			break;
		case ECommonInputType::Gamepad:
			if (CurrentInputType == ECommonInputType::Gamepad && InPressKey == EKeys::LeftMouseButton )
			{
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));

				check(InputActionData)

				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());
				
				return;
			}

			if (!InPressKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectedCanceled.ExecuteIfBound(TEXT("Delected non GamePad Key Pressed for Gamepad inputs, Key Remap has been cancled."));
				return;
			}


			break;
		
		default:
			break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressKey);

	}

private:

	ECommonInputType CachedInputTypeToListenTo;

	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};



void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputType,GetOwningLocalPlayer());
	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDelected);
	CachedInputPreprocessor->OnInputPreProcessorKeySelectedCanceled.BindUObject(this, &ThisClass::OnKeySelectCanceled);


	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);

	FString InputDeviceName;

	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("Mouse & Keyboard");

		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("Gamepad");
		break;
	
	default:
		break;
	}
	const FString DisplayRichMessage =  FString::Printf(
		TEXT("<KeyRemapDefault>Press any</><KeyRemapHighlight>%s</><KeyRemapDefault>key.</> "), *InputDeviceName);
    
	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));

}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);

		CachedInputPreprocessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPressedDelected(const FKey& PressKey)
{
	RequestDeactivateWidget(
		[this,PressKey]() {

			//FrontendUIDebugHelper::Log(TEXT("Pressed Key:") + PressKey.GetDisplayName().ToString());
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget(
		[this, CanceledReason]() {
			//FrontendUIDebugHelper::Log(CanceledReason);
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	//Delay a tick to make sure the input is processed correctly
	FTSTicker::GetCoreTicker().AddTicker(

		FTickerDelegate::CreateLambda(
			[PreDeactivateCallback,this](float DeltaTime)->bool {

				PreDeactivateCallback();

				DeactivateWidget();
				return false;
			}
		)
	);
}
