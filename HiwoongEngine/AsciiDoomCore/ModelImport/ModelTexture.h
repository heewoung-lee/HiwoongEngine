#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Hiwoong
{
    struct ModelTexture
    {
        std::size_t width = 0;
        std::size_t height = 0;

        // 픽셀마다 R, G, B 순서로 저장.
        std::vector<std::uint8_t> rgbPixels;
    };
}