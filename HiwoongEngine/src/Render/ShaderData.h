#pragma once

#include <vector>
#include <Windows.h>
namespace Hiwoong::ShaderData
{
    inline const std::vector<char> DetailedShadeCharacters =
    {
        ' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'
    };

    inline const COLORREF ColorPalette[16] = {
    RGB(0,   0,   0),
    RGB(0,   0,   128),
    RGB(0,   128, 0),
    RGB(0,   128, 128),
    RGB(128, 0,   0),
    RGB(128, 0,   128),
    RGB(128, 128, 0),
    RGB(192, 192, 192),
    RGB(128, 128, 128),
    RGB(0,   0,   255),
    RGB(0,   255, 0),
    RGB(0,   255, 255),
    RGB(255, 0,   0),
    RGB(255, 0,   255),
    RGB(255, 255, 0),
    RGB(255, 255, 255)
    };
}