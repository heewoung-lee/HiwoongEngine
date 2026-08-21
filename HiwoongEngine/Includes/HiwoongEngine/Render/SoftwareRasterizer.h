#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include <vector>
namespace Hiwoong
{
    class Hiwoong_API SoftwareRasterizer
    {
    public:
        //Translate screenPosition
        static Vector2 NdcToScreen(
            const Vector3& ndcPosition,
            int screenWidth,
            int screenHeight
        );

        static std::vector<Vector2> RasterizeLine(
            const Vector2& start,
            const Vector2& end
        );
    };
}