#pragma once

#include "Core/Core.h"
#include <Windows.h>

namespace Hiwoong
{
	enum class Hiwoong_API Color : WORD
	{
        Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
        Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE |
        FOREGROUND_INTENSITY,
        Yellow = FOREGROUND_RED | FOREGROUND_GREEN |
        FOREGROUND_INTENSITY,
        Magenta = FOREGROUND_RED | FOREGROUND_BLUE |
        FOREGROUND_INTENSITY,

        Orange = FOREGROUND_RED | FOREGROUND_GREEN,

        Gray = FOREGROUND_RED | FOREGROUND_GREEN |
        FOREGROUND_BLUE,

        White = Gray | FOREGROUND_INTENSITY
	};
}