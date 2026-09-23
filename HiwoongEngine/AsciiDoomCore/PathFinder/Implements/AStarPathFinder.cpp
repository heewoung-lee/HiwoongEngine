#include "AStarPathFinder.h"
#include <cstdlib>
namespace Hiwoong
{
	namespace
	{
		struct PathNode
		{
			//이 노드의 격자위치
			GridPosition position;

			//시작점에서 여기까지 이동한 거리
			int gCost = 0;

			//여기서 목적지까지 남은 예상거리
			int hCost = 0;

			//이전칸
			GridPosition previousPosition;

			//다음에 조사할 노드의 점수를 고른다.
			int GetFCost() const
			{
				return gCost + hCost;
			}
		};
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
				if (nodes[index].position == position) return index;
			}

			return -1;
		}


		int FindBestNodeIndex(const std::vector<PathNode>& openNodes)
		{
			int bestIndex = 0;

			//1로 시작하는 이유는 처음 순회할때 베스트 인덱스가 0 이기 때문에 베스트와 비교를 하기 때문
			for (int index = 1; index < static_cast<int>(openNodes.size());++index)
			{
				//두개를 비교
				const PathNode& candidate = openNodes[index];
				const PathNode& best = openNodes[bestIndex];

				if (candidate.GetFCost() < best.GetFCost())
				{
					bestIndex = index;
				}//만약 FCost의 결과가 같다면 h코스트(남은 예상거리) 비교로 bestIndex를 찾는다.
				else if(candidate.GetFCost() == best.GetFCost() && candidate.hCost < best.hCost)
				{
					bestIndex = index;
				}
			}

			return bestIndex;
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


