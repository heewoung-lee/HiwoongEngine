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
		/// <summary>
		/// 이전노드를 통해 현재 노드들을 계속갱신하면서 목적지 까지 다 다르면
		/// 목적지-> 출발지의 경로가 나오고 그 이후 뒤집으면 최소경로가 나옴
		/// </summary>
		/// <param name="closedNodes"></param>
		/// <param name="start"></param>
		/// <param name="target"></param>
		/// <returns></returns>
		std::vector<GridPosition> BuildPath(
			const std::vector<PathNode>& closedNodes,
			const GridPosition& start,
			const GridPosition& target
		)
		{
			std::vector<GridPosition> path;
			GridPosition current = target;

			while (current != start)
			{
				path.push_back(current);

				const int currentIndex =
					FindNodeIdx(closedNodes, current);

				if (currentIndex == -1)
				{
					return {};
				}

				current =
					closedNodes[currentIndex].GetPreviousPosition();
			}

			path.push_back(start);

			std::reverse(path.begin(), path.end());

			return path;
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

		//앞으로 볼곳
		AStarPriorityQueue openNodes;
		//이미 본곳
		std::vector<PathNode> closedNodes;

		openNodes.push(PathNode(
			start,
			0,
			CalculateManhattanDistance(start, target),
			start
		));

		while (openNodes.empty() == false)
		{
			//우선 순위가 제일 높은 노드를 가져와서 보관
			const PathNode currentNode = openNodes.top();
			openNodes.pop();

			//이미 가본칸이면 무시.
			if (FindNodeIdx(
				closedNodes,
				currentNode.GetPosition()
			) != -1)
			{
				continue;
			}


			closedNodes.push_back(currentNode);

			if (currentNode.GetPosition() == target)
			{
				return BuildPath(
					closedNodes,
					start,
					target
				);
			}

			//각 이웃에 대해 벽이나 맵 밖이면 버리고, 
			// 이미 조사한 칸도 버린 뒤, 
			// 이동 가능한 새 칸만 Open에 넣는다.
			for (const GridPosition& neighbor :
				GetNeighbors(currentNode.GetPosition()))
			{
				if (grid.IsWalkable(neighbor) == false)
				{
					continue;
				}

				if (FindNodeIdx(closedNodes, neighbor) != -1)
				{
					continue;
				}

				const int newGCost =
					currentNode.GetGCost() + 1;

				openNodes.push(PathNode(
					neighbor,
					newGCost,
					CalculateManhattanDistance(neighbor, target),
					currentNode.GetPosition()
				));
			}
		}
		return {};
	}
}


