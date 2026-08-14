#pragma once
#include "Math/Vector2.h"
#include <vector>
namespace Hiwoong
{
	class TetrisBoard
	{
	public:
		TetrisBoard(int width, int height);

		bool IsOccupied(const Vector2& position) const;
		void SetOccupied(const Vector2& position, bool occupied);


	private :
		int width;
		int height;
		std::vector<bool> cells;

	};

}