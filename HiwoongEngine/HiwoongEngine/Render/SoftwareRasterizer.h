#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include <vector>
namespace Hiwoong
{
    //화면 밖까지 렌더링을 하게 되면, 
    // 계산량이 커지므로 불필요한 계산을 줄이기 위해 삼각형의 검사범위 구조체 선언
    //이걸로 범위를 계산하는 메서드와 범위를 순회해서 화면 밖은 아예 렌더링에서 제외시킴.
    struct RasterBounds
    {
        int minX;
        int maxX;
        int minY;
        int maxY;
    };


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
            const Vector2& point2,
            int screenWidth,
            int screenHeight
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

        // 삼각형과 화면이 겹치는 검사 범위를 계산하는 메서드.
        // 예를 들어 삼각형의 X 범위가 -100~40, 화면이 0~59라면 0~40만 검사하고
        // 이후 0~40 구간만 렌더링 
        static RasterBounds CalculateRasterBounds(
            const Vector2& point0,
            const Vector2& point1,
            const Vector2& point2,
            int screenWidth,
            int screenHeight
        );


    };
}