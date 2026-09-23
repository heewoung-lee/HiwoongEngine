#pragma once

#include "GridPosition.h"

#include <cstddef>
#include <vector>


namespace Hiwoong
{
	class NavigationGrid
	{
	public:
		explicit NavigationGrid(const std::vector<std::vector<bool>>& walkableTiles);


		bool IsWalkable(const GridPosition& position) const;

		std::size_t GetWidth() const;
		std::size_t GetHeight() const;

	private:
		bool IsInside(const GridPosition& position) const;

		std::vector<std::vector<bool>> walkableTiles;
		bool isValid = false;//빈맵이나 잘못된 데이터가 들어올때 판별
	};
}
