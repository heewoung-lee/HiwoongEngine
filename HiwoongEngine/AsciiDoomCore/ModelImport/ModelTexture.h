#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
namespace Hiwoong
{
    struct ModelTextureInfo
    {
        std::string filePath;
        bool repeatU = false; // 가로 반복
        bool repeatV = false; // 세로 반복
    };

    struct ModelTexture
    {
        std::size_t width = 0;
        std::size_t height = 0;

        //텍스쳐 이미지정보
        ModelTextureInfo info;
        // 픽셀마다 R, G, B 순서로 저장.
        std::vector<std::uint8_t> rgbPixels;
    };
}