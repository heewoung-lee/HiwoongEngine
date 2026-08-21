#pragma once

#include "Core/Core.h"
#include "Render/Vertex.h"
#include "Render/Edge.h"
#include <vector>
namespace Hiwoong
{
	struct Hiwoong_API Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<Edge> edges;
	};
}

