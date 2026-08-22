#pragma once

#include "Core/Core.h"

namespace Hiwoong
{
	class Hiwoong_API Vector3
	{
	public:
		Vector3(float newX, float newY, float newZ);
		~Vector3() = default;


		static Vector3 Zero;
		

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator*(float value) const;
		Vector3 operator/(const Vector3& other) const;

		Vector3& operator= (const Vector3& other);

		float Length() const;
		

		//Measures How similar the direction of a and b are.
		static float Dot(const Vector3& a, const Vector3& b);

		//Returns a vector perpendicular to both a and b
		//I use right-handed coordinate system
		static Vector3 Cross(const Vector3& a, const Vector3& b);

		Vector3 Normalized() const;

		bool operator == (const Vector3& other) const;
		bool operator != (const Vector3& other) const;

		float x;
		float y;
		float z;
	};
}

