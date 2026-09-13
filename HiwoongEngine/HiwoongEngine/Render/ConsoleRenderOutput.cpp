#include "ConsoleRenderOutput.h"
#include "ScreenBuffer.h"


#include <cassert>
#include <utility>

namespace Hiwoong
{
    ConsoleRenderOutput::ConsoleRenderOutput(
        const ConsoleRenderOptions& options)
        : options(options)
    {
    }
    ConsoleRenderOutput::~ConsoleRenderOutput()
    {
        // 기존 콘솔 화면으로 돌아간 뒤 버퍼들이 해제된다.
        SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    }

    void ConsoleRenderOutput::Resize(const Vector2& screenSize)
    {
        auto first = std::make_unique<ScreenBuffer>(screenSize, options);
        auto second = std::make_unique<ScreenBuffer>(screenSize, options);

        first->Clear();
        second->Clear();

        // 새 버퍼를 표시한 뒤 기존 버퍼를 교체한다.
        const BOOL result =
            SetConsoleActiveScreenBuffer(first->GetScreenBuffer());

        assert(result != FALSE);

        buffers[0] = std::move(first);
        buffers[1] = std::move(second);
        currentBufferIndex = 0;

        characterSize = Vector2::Zero;

        if (!buffers[0]->TryGetCharacterSize(characterSize))
        {
            OutputDebugStringA(
                "[ConsoleFont] Character size unavailable.\n"
            );
        }
    }
    void ConsoleRenderOutput::Present(const CHAR_INFO* cells)
    {
        assert(cells != nullptr);
        assert(buffers[currentBufferIndex] != nullptr);

        auto& buffer = buffers[currentBufferIndex];

        // 준비된 문자·색상 배열을 현재 버퍼에 쓴다.
        buffer->Draw(cells);

        // 작성한 버퍼를 화면에 표시한다.
        const BOOL result =
            SetConsoleActiveScreenBuffer(buffer->GetScreenBuffer());

        assert(result != FALSE);

        // 다음 출력에는 반대쪽 버퍼를 사용한다.
        currentBufferIndex = 1 - currentBufferIndex;
    }

}