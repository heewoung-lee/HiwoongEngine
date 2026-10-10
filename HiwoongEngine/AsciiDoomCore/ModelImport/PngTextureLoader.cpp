#include "PngTextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../ThirdParty/stb_image.h"

namespace Hiwoong
{
    bool LoadPngTexture(
        const std::string& filePath,
        ModelTexture& texture
    )
    {
        int width = 0;
        int height = 0;

        stbi_uc* pixels = stbi_load(
            filePath.c_str(),
            &width,
            &height,
            nullptr,
            STBI_rgb
        );

        if (pixels == nullptr)
            return false;

        texture.width = static_cast<std::size_t>(width);
        texture.height = static_cast<std::size_t>(height);

        const std::size_t byteCount =
            texture.width * texture.height * 3;

        texture.rgbPixels.assign(pixels, pixels + byteCount);

        stbi_image_free(pixels);
        return true;
    }
}