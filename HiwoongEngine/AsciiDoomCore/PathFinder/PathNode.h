#pragma once

#include "GridPosition.h"
#include <vector>

namespace Hiwoong
{
	struct PathNode
	{

	public:
		
		PathNode(GridPosition position, int gCost, int hCost, GridPosition prePosition);

		//다음에 조사할 노드의 점수를 고른다.
		int GetFCost() const;
		bool IsLowerThanOther(const PathNode& other) const;
		inline const GridPosition& GetPreviousPosition() const
		{
			return previousPosition;
		}
		inline const GridPosition& GetPosition()const { return position; }
		inline const int GetHCost() const { return hCost; }
		inline int GetGCost() const{return gCost;}
	private:
		//이 노드의 격자위치
		GridPosition position;

		//시작점에서 여기까지 이동한 거리
		int gCost = 0;

		//여기서 목적지까지 남은 예상거리
		int hCost = 0;

		//이전칸
		GridPosition previousPosition;
	};


	class AStarPriorityQueue
	{
	public:
		inline bool empty() const { return nodeList.empty(); }
		const PathNode& top() const;
		void push(PathNode node);
		void pop();

	private:
		std::vector<PathNode> nodeList;
		void UpOrganizing(size_t index);
		void DownOrganizing();
	};


}
