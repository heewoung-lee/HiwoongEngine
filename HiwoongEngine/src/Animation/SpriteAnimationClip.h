#pragma once

#include <string>
#include <vector>

namespace Hiwoong
{
    struct SpriteAnimationClip
    {
        std::vector<std::vector<std::string>> frames;//애니메이션
        double duration = 1.0; //몇초동안 재생할껀지.
    };
}