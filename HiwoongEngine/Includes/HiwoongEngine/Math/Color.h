#pragma once

#include "Core/Core.h"
#include <Windows.h>

namespace Hiwoong
{
	enum class Hiwoong_API Color : WORD
	{
		Red = FOREGROUND_RED,
		Green = FOREGROUND_GREEN,
		Blue = FOREGROUND_BLUE,
		White = Red | Green | Blue,
		Cyan = Green | Blue,
		Yellow = Red | Green,
		BrightWhite = White | FOREGROUND_INTENSITY,
	};
}