#pragma once

#include "Core/Core.h"


namespace Hiwoong
{
	class Hiwoong_API Vector4
	{
	public:
		Vector4(float newX, float newY, float newZ, float newW);
		~Vector4() = default;

		float x;
		float y;
		float z;
		float w;
	};
}
