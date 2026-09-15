#pragma once

#include "Core/Core.h"
#include "Render/Mesh.h"

namespace Hiwoong
{
	class Hiwoong_API MeshFactory
	{
	public:
		static Mesh CreateCube(float size);
	};
}
