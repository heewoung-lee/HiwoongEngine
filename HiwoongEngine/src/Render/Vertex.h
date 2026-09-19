#pragma once

#include "Core/Core.h"
#include "Math/Vector3.h"
namespace Hiwoong
{
	struct Hiwoong_API Vertex
	{
		Vector3 position;

		float u = 0.0f;//가로 위치
		float v = 0.0f;//세로 위치

	};
}