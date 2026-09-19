#pragma once

#include "Core/Core.h"
#include "Render/Mesh.h"

namespace Hiwoong
{
	class Hiwoong_API MeshFactory
	{
	public:
		static Mesh CreateCube(float size);
		static Mesh CreateSphere(
			float radius,
			int sliceCount,
			int stackCount
		);
		static Mesh CreateQuad(float width, float height);
	};
}
