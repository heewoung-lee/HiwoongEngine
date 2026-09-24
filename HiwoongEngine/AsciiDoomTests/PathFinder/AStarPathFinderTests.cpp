#include "AStarPathFinderTests.h"
#include "PathFinder/NavigationGrid.h"
#include "PathFinder/Implements/AStarPathFinder.h"
#include "PathFinder/PathNode.h"

#include <vector>

void Hiwoong::Tests::RunAStarPathFinderTests(TestRunner& testRunner)
{
	const std::vector<std::vector<bool>> tiles = 
	{
		{true}
	};

	const NavigationGrid grid(tiles);
	const AStarPathFinder pathFinder;

	const std::vector<GridPosition> path =
		pathFinder.FindPath(
			grid,
			{ 0,0 },
			{ 0,0 }
		);

	testRunner.Check(
		path.size() == 1 &&
		path.front() == GridPosition{0,0},
		"AStar returns start when start equals target"
	);

	const std::vector<std::vector<bool>> straightTiles =
	{
		{ true, true, true }
	};

	const NavigationGrid straightGrid(straightTiles);

	const std::vector<GridPosition> straightPath =
		pathFinder.FindPath(
			straightGrid,
			{ 0, 0 },
			{ 0, 2 }
		);

	testRunner.Check(
		straightPath.size() == 3 &&
		straightPath[0] == GridPosition{ 0, 0 } &&
		straightPath[1] == GridPosition{ 0, 1 } &&
		straightPath[2] == GridPosition{ 0, 2 },
		"AStar finds straight path"
	);

	const std::vector<std::vector<bool>> detourTiles =
	{
		{ true, false, true },
		{ true, true,  true }
	};

	const NavigationGrid detourGrid(detourTiles);

	const std::vector<GridPosition> detourPath =
		pathFinder.FindPath(
			detourGrid,
			{ 0, 0 },
			{ 0, 2 }
		);

	testRunner.Check(
		detourPath.size() == 5 &&
		detourPath[0] == GridPosition{ 0, 0 } &&
		detourPath[1] == GridPosition{ 1, 0 } &&
		detourPath[2] == GridPosition{ 1, 1 } &&
		detourPath[3] == GridPosition{ 1, 2 } &&
		detourPath[4] == GridPosition{ 0, 2 },
		"AStar finds path around wall"
	);

	 
	AStarPriorityQueue queue;



	queue.push(PathNode{
	{ 0, 1 },
	5,
	6,
	{ 0, 0 }
		});

	queue.push(PathNode{
		{ 0, 0 },
		5,
		5,
		{ 0, 0 }
		});

	queue.push(PathNode{
		{ 0, 2 },
		1,
		2,
		{ 0, 0 }
		});

	queue.push(PathNode{
	{ 0, 9 },
	0,
	0,
	{ 0, 0 }
		});


	queue.push(PathNode{
	{ 0, 3 },
	7,
	6,
	{ 0, 0 }
		});


	testRunner.Check(
		queue.top().GetPosition() == GridPosition{ 0, 9 },
		"PriorityQueue returns lowest-cost node"
	);

	queue.pop();

	testRunner.Check(
		queue.top().GetPosition() == GridPosition{ 0, 2 },
		"PriorityQueue restores heap after pop"
	);

}
