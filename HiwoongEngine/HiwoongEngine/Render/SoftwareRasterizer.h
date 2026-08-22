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
        //Fill Triangles
        static std::vector<Vector2> RasterizeTriangle(
            const Vector2& point0,
            const Vector2& point1,
            const Vector2& point2
        );

        // Calculate how much each triangle vertex contributes to the point.
        static Vector3 CalculateBarycentric(
            const Vector2& point,
            const Vector2& point0,
            const Vector2& point1,
            const Vector2& point2
        );

        // Return true when the triangle faces away from the camera.
        static bool IsBackFace(
            const Vector2& point0,
            const Vector2& point1,
            const Vector2& point2
        );

    };
}