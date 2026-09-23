#include "NavigationGridTests.h"

#include <vector>
#include "PathFinder/NavigationGrid.h"

void Hiwoong::Tests::RunNavigationGridTests(TestRunner& testRunner)
{
	const std::vector<std::vector<bool>> tiles =
	{
		{true,false,true},
		{true,true,true}
	};

	const NavigationGrid grid(tiles);

	testRunner.Check(
		grid.IsWalkable({ 0, 1 }) == false,
		"NavigationGrid blocked tile"
	);

	testRunner.Check(
		grid.IsWalkable({ -1, 0 }) == false,
		"NavigationGrid rejects negative row"
	);

	testRunner.Check(
		grid.IsWalkable({ 0, -1 }) == false,
		"NavigationGrid rejects negative column"
	);

	testRunner.Check(
		grid.IsWalkable({ 2, 0 }) == false,
		"NavigationGrid rejects row overflow"
	);

	testRunner.Check(
		grid.IsWalkable({ 0, 3 }) == false,
		"NavigationGrid rejects column overflow"
	);

	testRunner.Check(
		grid.GetWidth() == 3,
		"NavigationGrid width"
	);

	testRunner.Check(
		grid.GetHeight() == 2,
		"NavigationGrid height"
	);

	testRunner.Check(
		grid.IsWalkable({ 0, 0 }) == true,
		"NavigationGrid walkable tile"
	);

	const std::vector<std::vector<bool>> emptyTiles;
	const NavigationGrid emptyGrid(emptyTiles);

	testRunner.Check(
		emptyGrid.GetWidth() == 0 &&
		emptyGrid.GetHeight() == 0 &&
		emptyGrid.IsWalkable({ 0,0 }) == false,
		"NavigationGrid rejects empty grid"
	);


	const std::vector<std::vector<bool>> unvenTile = {
		{true,true},
		{false}
	};

	const NavigationGrid unvenGrid(unvenTile);

	testRunner.Check(
		unvenGrid.GetWidth() == 0 &&
		unvenGrid.GetHeight() == 0 &&
		unvenGrid.IsWalkable({ 0,0 }) == false,
		"NavigationGrid rejects uneven rows"
	);

}
