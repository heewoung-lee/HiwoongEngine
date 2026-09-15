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

		static bool NearyEquals(const Vector4& a, const Vector4& b);
		Vector3 PerspectiveDivide() const;
		bool operator ==(const Vector4& other) const;

		

		float x;
		float y;
		float z;
		float w;
	};
}
