#include "TetrisBoard.h"
#include <cassert>

namespace Hiwoong
{
	TetrisBoard::TetrisBoard(int width, int height): width(width), height(height),cells(width*height,false)
	{
		assert(width > 0);
		assert(height > 0);
	}
	bool TetrisBoard::IsOccupied(const Vector2& position) const
	{

		//wall is true
		if (position.x < 0 || position.x >= width) return true;

		if (position.y < 0 || position.y >= height) return true;

		const int idx = position.y * width + position.x;

		return cells[idx];
	}
    void TetrisBoard::SetOccupied(
        const Vector2& position,
        bool occupied)
    {
        if (position.x < 0 || position.x >= width)
            return;

        if (position.y < 0 || position.y >= height)
            return;

		const int idx = position.y * width + position.x;

        cells[idx] = occupied;
    }
}