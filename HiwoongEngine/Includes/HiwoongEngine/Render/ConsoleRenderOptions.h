#pragma once

#include "Math/Vector2.h"
#include <string>

namespace Hiwoong
{
    //9.13일 추가. 미친 프로젝트마다 요구하는 글꼴크기를 미쳐 생각 못해서 만듦.
    struct ConsoleRenderOptions
    {
        // 비어 있으면 기존 글꼴을 유지한다.
        std::wstring fontName;

        // (0, 0)이면 기존 크기를 유지한다.
        // 단위는 문자 한 칸의 픽셀 크기다.
        Vector2 characterSize = Vector2::Zero;
    };
}