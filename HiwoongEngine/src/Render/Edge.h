#pragma once

#include "Core/Core.h"
#include <cstddef>

namespace Hiwoong
{
    struct Hiwoong_API Edge
    {
        std::size_t startIndex;
        std::size_t endIndex;
    };
}