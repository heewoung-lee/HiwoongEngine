#include "SoftwareRasterizer.h"
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
}