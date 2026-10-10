#pragma once

#include "ModelTexture.h"
#include <string>

namespace Hiwoong
{
    bool LoadPngTexture(
        const std::string& filePath,
        ModelTexture& texture
    );
}