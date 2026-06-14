#pragma once

/**
 * 前端 UI 调试辅助命名空间
 * 提供在屏幕上输出调试日志的静态工具函数。
 */
namespace FrontendUIDebugHelper
{
	/**
	 * 在屏幕上输出调试日志
	 * @param Message   要显示的调试消息
	 * @param Inkey     消息键值（-1 表示每次使用新键，不覆盖之前的消息）
	 * @param TextColor 文本颜色（默认为随机颜色）
	 */
	static void Log(const FString& Message,int32 Inkey = -1,const FColor& TextColor = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(Inkey, 5.f, TextColor, Message);
		}
	}
}
