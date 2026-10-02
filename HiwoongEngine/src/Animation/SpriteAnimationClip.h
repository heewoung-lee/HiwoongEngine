#pragma once

#include <string>
#include <vector>

namespace Hiwoong
{
    struct SpriteAnimationClip
    {
        std::vector<std::vector<std::string>> frames;//애니메이션
        double duration = 1.0; //몇초동안 재생할껀지.
        bool isLooping = false; //IDLE같은 애니메이션을 위한 무한재생
        bool lockUntilFinished = false;// true면 전체 재생이 끝날 때까지 다른 클립으로 전환하지 않는다.
    };
}