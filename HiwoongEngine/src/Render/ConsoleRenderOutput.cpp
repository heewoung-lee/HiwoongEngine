#include "ConsoleRenderOutput.h"
#include "ScreenBuffer.h"
#include <climits>
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

        consoleCells.resize(screenSize.x * screenSize.y);
        
        characterSize = Vector2::Zero;

        if (!buffers[0]->TryGetCharacterSize(characterSize))
        {
            OutputDebugStringA(
                "[ConsoleFont] Character size unavailable.\n"
            );
        }
    }
    WORD ConsoleRenderOutput::ToConsoleColor(Color color)
    {
        // 콘솔 색상 번호 0~15에 대응하는 RGB
        static const Color palette[16] =
        {
            { 0, 0, 0 },
            { 0, 0, 128 },
            { 0, 128, 0 },
            { 0, 128, 128 },
            { 128, 0, 0 },
            { 128, 0, 128 },
            { 128, 128, 0 },
            { 192, 192, 192 },
            { 128, 128, 128 },
            { 0, 0, 255 },
            { 0, 255, 0 },
            { 0, 255, 255 },
            { 255, 0, 0 },
            { 255, 0, 255 },
            { 255, 255, 0 },
            { 255, 255, 255 }
        };

        int bestDistance = INT_MAX;
        WORD bestColor = 0;

        for (WORD i = 0; i < 16; ++i)
        {
            const int redDifference = color.red - palette[i].red;
            const int greenDifference = color.green - palette[i].green;
            const int blueDifference = color.blue - palette[i].blue;

            const int distance =
                redDifference * redDifference +
                greenDifference * greenDifference +
                blueDifference * blueDifference;

            if (distance < bestDistance)
            {
                bestDistance = distance;
                bestColor = i;
            }
        }

        return bestColor;
    }
    void ConsoleRenderOutput::Present(const RenderCell* cells)
    {
        assert(cells != nullptr);
        assert(buffers[currentBufferIndex] != nullptr);

        auto& buffer = buffers[currentBufferIndex];

        // RGB 입력을 콘솔용 문자와 색상 번호로 변환한다.
        for (std::size_t i = 0; i < consoleCells.size(); ++i)
        {
            consoleCells[i].Char.AsciiChar = cells[i].character;
            consoleCells[i].Attributes = ToConsoleColor(cells[i].color);
        }

        buffer->Draw(consoleCells.data());

        const BOOL result =
            SetConsoleActiveScreenBuffer(buffer->GetScreenBuffer());

        assert(result != FALSE);

        currentBufferIndex = 1 - currentBufferIndex;
    }

}