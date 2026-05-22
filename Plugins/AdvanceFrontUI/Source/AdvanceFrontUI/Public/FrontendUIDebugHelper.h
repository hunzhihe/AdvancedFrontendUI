#pragma once

namespace FrontendUIDebugHelper
{
	// Debug log for Frontend UI
	static void Log(const FString& Message,int32 Inkey = -1,const FColor& TextColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 5.f, TextColor, Message);
		}
	}
}