#pragma once
#include "PathNode.h"
#include "GridPosition.h"
#include <vector>
#include <cassert>
namespace Hiwoong
{
	PathNode::PathNode
		(GridPosition position, int gCost, int hCost, GridPosition prePosition):
		position(position),gCost(gCost),hCost(hCost), previousPosition(prePosition)
	{

	}
	//다음에 조사할 노드의 점수를 고른다.
	int PathNode::GetFCost() const
	{
		return gCost + hCost;
	}

	bool PathNode::IsLowerThanOther(const PathNode& other) const
	{
		if (GetFCost() == other.GetFCost())
		{
			return hCost < other.hCost;
		}
		return GetFCost() < other.GetFCost();
	}


	/////////아래는 우선순위 큐 구현
	const PathNode& AStarPriorityQueue::top() const
	{
		assert(nodeList.empty() == false);

		return nodeList[0];
	}

	void AStarPriorityQueue::push(PathNode node)
	{
		nodeList.push_back(node);
		UpOrganizing(nodeList.size() - 1);
	}

	void AStarPriorityQueue::pop()
	{
		if (nodeList.empty()) return;

		nodeList[0] = nodeList[nodeList.size() - 1];
		nodeList.pop_back();
		DownOrganizing();
	}

	void AStarPriorityQueue::UpOrganizing(size_t index)
	{
		while (index > 0)
		{
			size_t parentidx = (index - 1) / 2;
			//내가 부모보다 더 크다면 교체해야함.
			if (nodeList[index].IsLowerThanOther(nodeList[parentidx]))
			{
				PathNode temp = nodeList[index];
				nodeList[index] = nodeList[parentidx];
				nodeList[parentidx] = temp;

				index = parentidx;
			}
			else break;
		}
	}
	void AStarPriorityQueue::DownOrganizing()
	{
		size_t curidx = 0;

		while (true)
		{
			size_t index = curidx;
			size_t leftChild = (index * 2) + 1;
			size_t rightChild = (index * 2) + 2;

			if (leftChild < nodeList.size() && nodeList[leftChild].IsLowerThanOther(nodeList[index]))
			{
				index = leftChild;
			}

			if (rightChild < nodeList.size() && nodeList[rightChild].IsLowerThanOther(nodeList[index]))
			{
				index = rightChild;
			}
			if (index != curidx)
			{
				PathNode temp = nodeList[index];
				nodeList[index] = nodeList[curidx];
				nodeList[curidx] = temp;

				curidx = index;
			}
			else break;
		}

	}
};
