#pragma once

#include "../ModelImport/SkeletonNode.h"
#include <vector>

namespace Hiwoong
{
    struct AnimationFrame
    {
        // 애니메이션 시작부터 지난 시간.
        // 즉 시간마다 관절의 위치를 확인해서 정점위치를 잡을 수 있음
        double time = 0.0;

        // 이 시점의 전체 관절 위치, 회전, 크기.
        std::vector<SkeletonNode> nodes;
    };
}