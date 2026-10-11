#pragma once

#include "AnimationFrame.h"
#include <string>
#include <vector>

namespace Hiwoong
{
    struct AnimationClip
    {
        // 애니메이션 이름.
        std::string name;

        // 전체 재생 길이. 단위는 초.
        double duration = 0.0;

        // 시간순으로 저장한 관절 자세들.
        std::vector<AnimationFrame> frames;

        // 모델의 각 관절에 대응하는 이 클립의 관절 번호.
        std::vector<int> sourceNodeIndices;
    };
}