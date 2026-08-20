#pragma once

#include "Core/Core.h"
#include "Math/Vector3.h"


namespace Hiwoong
{
	class Hiwoong_API Vector4
	{
	public:
		Vector4(float newX, float newY, float newZ, float newW);
		~Vector4() = default;


		Vector3 PerspectiveDivide() const;

		float x;
		float y;
		float z;
		float w;
	};
}
