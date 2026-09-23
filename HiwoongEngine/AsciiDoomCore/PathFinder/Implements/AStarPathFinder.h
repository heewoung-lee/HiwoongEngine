#pragma once

#include "../Interfaces/IPathFinder.h"

namespace Hiwoong
{
	class AStarPathFinder final : public IPathFinder
	{
	public:
		std::vector<GridPosition> FindPath(
			const NavigationGrid& grid,
			const GridPosition& start,
			const GridPosition& target
		) const override;
	};
}

