#pragma once
#include "ConsoleRenderOptions.h"
#include "IRenderOutput.h"
#include "Core/Core.h"
#include <memory>

namespace Hiwoong
{
    class ScreenBuffer;

    //콘솔 화면
    class Hiwoong_API ConsoleRenderOutput : public IRenderOutput
    {
    public:
        explicit ConsoleRenderOutput(
            const ConsoleRenderOptions& options = {}
        );
        ~ConsoleRenderOutput() override;

        void Resize(const Vector2& screenSize) override;
        void Present(const CHAR_INFO* cells) override;

        Vector2 GetCharacterSize() const override
        {
            return characterSize;
        }

    private:
        std::unique_ptr<ScreenBuffer> buffers[2];
        int currentBufferIndex = 0;

        Vector2 characterSize = Vector2::Zero;
        ConsoleRenderOptions options;
    };
}