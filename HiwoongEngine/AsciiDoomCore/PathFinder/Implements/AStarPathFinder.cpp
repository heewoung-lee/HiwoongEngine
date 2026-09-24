#include "AStarPathFinder.h"
#include "../PathNode.h"
#include <cstdlib>
#include <vector>

namespace Hiwoong
{
	namespace
	{

		//맨해튼거리.
		int CalculateManhattanDistance
		(
			const GridPosition& from,
			const GridPosition& to
		)
		{
			return std::abs(from.row - to.row) + std::abs(from.column - to.column);
		}

		//상하좌우 배열 출력.
		std::vector<GridPosition> GetNeighbors(
			const GridPosition& position
		)
		{
			return
			{
				{ position.row - 1, position.column },
				{ position.row, position.column + 1 },
				{ position.row + 1, position.column },
				{ position.row, position.column - 1 }
			};
		}
		/// <summary>
		/// nodes에서 position이 있는 지 찾는다.
		/// </summary>
		/// <param name="nodes">다음으로 가기위해 모은 노드들</param>
		/// <param name="position">찾고 싶은 좌표</param>
		/// <returns></returns>
		int FindNodeIdx(
			const std::vector<PathNode>& nodes,
			const GridPosition& position
		)
		{
			for (int index = 0; index < static_cast<int>(nodes.size());index++)
			{
				if (nodes[index].GetPosition() == position) return index;
			}

			return -1;
		}


		

	}



	std::vector<GridPosition> AStarPathFinder::FindPath(
		const NavigationGrid& grid,
		const GridPosition& start,
		const GridPosition& target
	) const
	{
		if (grid.IsWalkable(start) == false ||
			grid.IsWalkable(target) == false)
		{
			return {};
		}

		if (start == target)
		{
			return { start };
		}

		if (start.row != target.row)
		{
			return {};
		}

		const int direction =
			start.column < target.column ? 1 : -1;

		std::vector<GridPosition> path;
		path.push_back(start);

		GridPosition current = start;

		while (current.column != target.column)
		{
			current.column += direction;

			if (grid.IsWalkable(current) == false)
			{
				return {};
			}

			path.push_back(current);
		}

		return path;
	}
}


