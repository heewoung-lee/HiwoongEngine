#pragma once

#include "ModelVertex.h"
#include "ModelTriangle.h"

#include <string>
#include <vector>

namespace Hiwoong
{
    struct ModelMesh
    {
        std::string name;
        std::vector<ModelVertex> vertices;
        std::vector<ModelTriangle> triangles;
    };
}