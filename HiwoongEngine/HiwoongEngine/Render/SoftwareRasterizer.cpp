#include "SoftwareRasterizer.h"
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <cmath>

namespace Hiwoong
{
    

	Vector2 SoftwareRasterizer::NdcToScreen(
		const Vector3& ndcPosition,
		int screenWidth,
		int screenHeight)
	{
		assert(screenWidth > 0);
		assert(screenHeight > 0);

		const float screenX =
			(ndcPosition.x + 1.0f) * 0.5f *
			(screenWidth - 1);

		const float screenY =
			(ndcPosition.y + 1.0f) * 0.5f *
			(screenHeight - 1);

		return Vector2(
			static_cast<int>(std::round(screenX)),
			static_cast<int>(std::round(screenY))
		);
	}

	//Return all of position
    //Use Bresenham Algorithm
    std::vector<Vector2> SoftwareRasterizer::RasterizeLine(
        const Vector2& start,
        const Vector2& end)
    {
        std::vector<Vector2> points;

        // Current raster position.
        int x = start.x;
        int y = start.y;

        // Calculate the horizontal distance and direction.
        const int deltaX =
            std::abs(end.x - start.x);

        const int stepX =
            start.x < end.x ? 1 : -1;

        // Calculate the vertical distance and direction.
        // deltaY is negative to balance both axes with one error value.
        const int deltaY =
            -std::abs(end.y - start.y);

        const int stepY =
            start.y < end.y ? 1 : -1;

        // Track how far the current cell is from the ideal line.
        int error = deltaX + deltaY;

        while (true)
        {
            // Add the current screen cell.
            points.emplace_back(x, y);

            if (x == end.x && y == end.y)
            {
                break;
            }
            // Stop after including the end point.
            const int twiceError = error * 2;

            if (twiceError >= deltaY)
            {
                error += deltaY;
                x += stepX;
            }
            // Move vertically when necessary.
            if (twiceError <= deltaX)
            {
                error += deltaX;
                y += stepY;
            }
        }
        return points;
    }
    int EdgeFunction(
        const Vector2& start,
        const Vector2& end,
        const Vector2& point)
    {
        return
            (point.x - start.x) * (end.y - start.y) -
            (point.y - start.y) * (end.x - start.x);
    }

    std::vector<Vector2> SoftwareRasterizer::RasterizeTriangle(
        const Vector2& point0,
        const Vector2& point1,
        const Vector2& point2)
    {
        if (EdgeFunction(point0, point1, point2) == 0)
        {
            return {};
        }

        const int minX = (std::min)({ point0.x, point1.x, point2.x });
        const int maxX = (std::max)({ point0.x, point1.x, point2.x });

        const int minY = (std::min)({ point0.y, point1.y, point2.y });
        const int maxY = (std::max)({ point0.y, point1.y, point2.y });

        std::vector<Vector2> points;

        for (int y = minY; y <= maxY; ++y)
        {
            for (int x = minX; x <= maxX; ++x)
            {
                const Vector2 point(x, y);

                const int edge0 = EdgeFunction(point0, point1, point);
                const int edge1 = EdgeFunction(point1, point2, point);
                const int edge2 = EdgeFunction(point2, point0, point);

                const bool hasNegative =
                    edge0 < 0 || edge1 < 0 || edge2 < 0;

                const bool hasPositive =
                    edge0 > 0 || edge1 > 0 || edge2 > 0;

                if (!(hasNegative && hasPositive))
                {
                    points.emplace_back(point);
                }
            }
        }

        return points;
    }
    Vector3 SoftwareRasterizer::CalculateBarycentric(
        const Vector2& point,
        const Vector2& point0,
        const Vector2& point1,
        const Vector2& point2)
    {
        const float area =
            static_cast<float>(
                EdgeFunction(point0, point1, point2)
                );

        assert(area != 0.0f);

        const float weight0 =
            EdgeFunction(point1, point2, point) / area;

        const float weight1 =
            EdgeFunction(point2, point0, point) / area;

        const float weight2 =
            EdgeFunction(point0, point1, point) / area;

        return Vector3(weight0, weight1, weight2);
    }
    bool SoftwareRasterizer::IsBackFace(
        const Vector2& point0,
        const Vector2& point1,
        const Vector2& point2)
    {
        const int signedArea =
            EdgeFunction(point0, point1, point2);

        return signedArea <= 0;
    }
}