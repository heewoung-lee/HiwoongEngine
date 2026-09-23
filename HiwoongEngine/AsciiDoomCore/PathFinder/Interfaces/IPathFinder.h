#pragma once

#include "../GridPosition.h"
#include "../NavigationGrid.h"

#include <vector>

namespace Hiwoong
{
	class IPathFinder
	{
	public:
		virtual ~IPathFinder() = default;

		virtual std::vector<GridPosition> FindPath(
			const NavigationGrid& grid,
			const GridPosition& start,
			const GridPosition& target
		) const = 0;
	};
}
