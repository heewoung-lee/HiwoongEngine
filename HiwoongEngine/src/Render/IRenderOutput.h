#pragma once

#include "Math/Vector2.h"
#include <Windows.h>

namespace Hiwoong
{
    class IRenderOutput
    {
    public:
        virtual ~IRenderOutput() = default;

        // 출력할 가로·세로 문자 칸 수를 설정한다.
        virtual void Resize(const Vector2& screenSize) = 0;

        // 완성된 문자·색상 배열을 화면에 표시한다.
        virtual void Present(const CHAR_INFO* cells) = 0;

        // 문자 한 칸의 픽셀 크기를 반환한다.
        virtual Vector2 GetCharacterSize() const = 0;

        // false이면 프로그램 종료 요청.
        virtual bool ProcessEvents() { return true; }
    };
}