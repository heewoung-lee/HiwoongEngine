#pragma once

#include "Core/Core.h"
#include <cstddef>

namespace Hiwoong
{
    //Vertex
    struct Hiwoong_API Triangle
    {
        std::size_t index0;
        std::size_t index1;
        std::size_t index2;
    };
}