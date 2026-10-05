#pragma once

#include <cstddef>

namespace Hiwoong
{
    //ModelVertex의 정점을 가져오면 어떤순서로 삼각형을 그릴지
    //정보를 담는 구조체
    struct ModelTriangle
    {
        std::size_t index0 = 0;
        std::size_t index1 = 0;
        std::size_t index2 = 0;
    };
}