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
bool TestCreateCubeHasTwelveTriangles()
{
	Hiwoong::Mesh cube =
		Hiwoong::MeshFactory::CreateCube(2.0f);

	return cube.triangles.size() == 12;
}

bool TestRasterizeTriangle()
{
	const std::vector<Hiwoong::Vector2> points =
		Hiwoong::SoftwareRasterizer::RasterizeTriangle(
			Hiwoong::Vector2(1, 1),
			Hiwoong::Vector2(3, 1),
			Hiwoong::Vector2(1, 3)
		);

	return points.size() == 6;
}
bool TestCalculateBarycentricAtTriangleCenter()
{
	const Hiwoong::Vector3 weights =
		Hiwoong::SoftwareRasterizer::CalculateBarycentric(
			Hiwoong::Vector2(1, 1),
			Hiwoong::Vector2(0, 0),
			Hiwoong::Vector2(3, 0),
			Hiwoong::Vector2(0, 3)
		);

	constexpr float expected = 1.0f / 3.0f;
	constexpr float epsilon = 0.0001f;

	return
		std::abs(weights.x - expected) < epsilon &&
		std::abs(weights.y - expected) < epsilon &&
		std::abs(weights.z - expected) < epsilon;
}

bool TestBackFaceCulling()
{
	const bool frontFace =
		Hiwoong::SoftwareRasterizer::IsBackFace(
			Hiwoong::Vector2(0, 0),
			Hiwoong::Vector2(2, 2),
			Hiwoong::Vector2(2, 0)
		);

	const bool backFace =
		Hiwoong::SoftwareRasterizer::IsBackFace(
			Hiwoong::Vector2(0, 0),
			Hiwoong::Vector2(2, 0),
			Hiwoong::Vector2(2, 2)
		);

	return frontFace == false &&
		backFace == true;
}

bool RunAllTests()
{
	return TestCreateCubeHasEightVertices() &&
		TestCreateCubeHasTwelveEdges() &&
		TestNdcCenterMapsToScreenCenter() &&
		TestNdcCornersMapToScreenCorners() &&
		TestRasterizeHorizontalLine() &&
		TestRasterizeVerticalLine() &&
		TestRasterizeDiagonalLine() &&
		TestCreateCubeHasTwelveTriangles() && 
		TestRasterizeTriangle()&&
		TestCalculateBarycentricAtTriangleCenter()&&
		TestBackFaceCulling();
}



int main()
{
	if (RunAllTests() == true)
	{
		std::cout << "Success" << std::endl;
	}
	else
	{
		std::cout << "failure" << std::endl;
	}
	
	/*Hiwoong::Engine engine;
	engine.AddNewScene<Hiwoong::CubeScene>();
	engine.Run();*/

	return 0;
}
