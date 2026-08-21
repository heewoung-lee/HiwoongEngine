#include <iostream>
#include "Render/MeshFactory.h"
#include "Render/SoftwareRasterizer.h"
#include "Math/Color.h"
#include "Engine/Engine.h"
#include "Test/CubeScene.h"

bool TestCreateCubeHasEightVertices()
{
	Hiwoong::Mesh cube =
		Hiwoong::MeshFactory::CreateCube(2.0f);

	return cube.vertices.size() == 8;
}
bool TestCreateCubeHasTwelveEdges()
{
	Hiwoong::Mesh cube =
		Hiwoong::MeshFactory::CreateCube(2.0f);

	return cube.edges.size() == 12;
}

bool TestNdcCenterMapsToScreenCenter()
{
	Hiwoong::Vector2 screenPosition =
		Hiwoong::SoftwareRasterizer::NdcToScreen(
			Hiwoong::Vector3(0, 0, 0),
			81,
			41
		);

	return screenPosition ==
		Hiwoong::Vector2(40, 20);
}

bool TestNdcCornersMapToScreenCorners()
{
	Hiwoong::Vector2 topLeft =
		Hiwoong::SoftwareRasterizer::NdcToScreen(
			Hiwoong::Vector3(-1, -1, 0),
			81,
			41
		);

	Hiwoong::Vector2 bottomRight =
		Hiwoong::SoftwareRasterizer::NdcToScreen(
			Hiwoong::Vector3(1, 1, 0),
			81,
			41
		);

	return topLeft == Hiwoong::Vector2(0, 0) &&
		bottomRight == Hiwoong::Vector2(80, 40);
}

bool TestRasterizeHorizontalLine()
{
	std::vector<Hiwoong::Vector2> points =
		Hiwoong::SoftwareRasterizer::RasterizeLine(
			Hiwoong::Vector2(2, 3),
			Hiwoong::Vector2(6, 3)
		);

	return points.size() == 5 &&
		points[0] == Hiwoong::Vector2(2, 3) &&
		points[1] == Hiwoong::Vector2(3, 3) &&
		points[2] == Hiwoong::Vector2(4, 3) &&
		points[3] == Hiwoong::Vector2(5, 3) &&
		points[4] == Hiwoong::Vector2(6, 3);
}
bool TestRasterizeVerticalLine()
{
	std::vector<Hiwoong::Vector2> points =
		Hiwoong::SoftwareRasterizer::RasterizeLine(
			Hiwoong::Vector2(3, 2),
			Hiwoong::Vector2(3, 6)
		);

	return points.size() == 5 &&
		points[0] == Hiwoong::Vector2(3, 2) &&
		points[1] == Hiwoong::Vector2(3, 3) &&
		points[2] == Hiwoong::Vector2(3, 4) &&
		points[3] == Hiwoong::Vector2(3, 5) &&
		points[4] == Hiwoong::Vector2(3, 6);
}

bool TestRasterizeDiagonalLine()
{
	std::vector<Hiwoong::Vector2> points =
		Hiwoong::SoftwareRasterizer::RasterizeLine(
			Hiwoong::Vector2(2, 2),
			Hiwoong::Vector2(6, 6)
		);

	return points.size() == 5 &&
		points[0] == Hiwoong::Vector2(2, 2) &&
		points[1] == Hiwoong::Vector2(3, 3) &&
		points[2] == Hiwoong::Vector2(4, 4) &&
		points[3] == Hiwoong::Vector2(5, 5) &&
		points[4] == Hiwoong::Vector2(6, 6);
}

bool RunAllTests()
{
	return TestCreateCubeHasEightVertices() &&
		TestCreateCubeHasTwelveEdges() &&
		TestNdcCenterMapsToScreenCenter() &&
		TestNdcCornersMapToScreenCorners() &&
		TestRasterizeHorizontalLine() &&
		TestRasterizeVerticalLine() &&
		TestRasterizeDiagonalLine();
}

int main()
{
	Hiwoong::Engine engine;

	engine.AddNewScene<Hiwoong::CubeScene>();
	engine.Run();
}
