#pragma once

#include "Math/Color.h"
#include <unordered_map>

namespace Hiwoong
{
	inline const std::unordered_map<char, Color> SpriteColorPalette = 
    {
       { 'R', Color::Red },
       { 'O', Color::Orange },
       { 'Y', Color::Yellow },
       { 'G', Color::Green },
       { 'B', Color::Blue },
       { 'C', Color::Cyan },
       { 'M', Color::Magenta },
       { 'W', Color::White },
       { 'S', Color::Gray },
       { 'K', Color::Black },
       { ' ', Color::White }
    };
}