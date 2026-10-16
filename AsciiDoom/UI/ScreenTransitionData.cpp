#include "ScreenTransitionData.h"
#include <cassert>
#include <string>
#include <vector>

namespace Hiwoong::ScreenTransitionData
{
    SpriteAnimationClip CreateClose(const Vector2& screenSize)
    {
        assert(screenSize.x > 0 && screenSize.y > 0);

        SpriteAnimationClip clip;
        clip.duration = 0.3;
        clip.isLooping = false;
        clip.lockUntilFinished = true;

        const int frameCount = 12;
        const float centerX = screenSize.x * 0.5f;
        const float centerY = screenSize.y * 0.5f;

        const std::vector<std::string> colorRows(
            screenSize.y,
            std::string(screenSize.x, 'K')
        );

        for (int frame = 0; frame < frameCount; ++frame)
        {
            const float progress =
                static_cast<float>(frame) / (frameCount - 1);

            const float remaining = 1.0f - progress;

            // 처음에는 화면 모서리까지 열린 크기를 받는다
            // 중심은 0, 가장자리는 -1~1로 바꿨기 때문에 X2로 받는다.
            const float radiusSquared =
                2.0f * remaining * remaining;

            std::vector<std::string> characterRows(
                screenSize.y,
                std::string(screenSize.x, ' ')
            );

            for (int row = 0; row < screenSize.y; ++row)
            {
                const float y =
                    (row + 0.5f - centerY) / centerY;

                for (int column = 0; column < screenSize.x; ++column)
                {
                    const float x =
                        (column + 0.5f - centerX) / centerX;

                    // 열린 영역 바깥은 검은색으로 가린다.
                    if (x * x + y * y >= radiusSquared)
                    {
                        characterRows[row][column] = '#';
                    }
                }
            }

            clip.frames.push_back(
                SpriteFrame(characterRows, colorRows)
            );
        }

        return clip;
    }
}