#pragma once

#include "Core/Core.h"
#include <cstdint>

namespace Hiwoong
{
    //10.20 컬러 방식 수정
    struct Hiwoong_API Color
    {
        std::uint8_t red = 0;
        std::uint8_t green = 0;
        std::uint8_t blue = 0;

        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Cyan;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Orange;
        static const Color Gray;
        static const Color White;
        static const Color BrightWhite;
    };
}