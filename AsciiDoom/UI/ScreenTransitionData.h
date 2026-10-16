#pragma once

#include "Animation/SpriteAnimationClip.h"
#include "Math/Vector2.h"

namespace Hiwoong::ScreenTransitionData
{
    //게임 종료시 재생할 애니메이션
    SpriteAnimationClip CreateClose(
        const Vector2& screenSize
    );
}