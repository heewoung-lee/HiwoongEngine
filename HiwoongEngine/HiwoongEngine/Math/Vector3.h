#pragma once

#include "Core/Core.h"

namespace Hiwoong
{
	constexpr float Epsilon = 0.0001f;

	class Hiwoong_API Vector3
	{
	public:
		Vector3(float newX = 0, float newY = 0, float newZ = 0);
		~Vector3() = default;


		static Vector3 Zero;
		

		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator*(float value) const;
		Vector3 operator/(const Vector3& other) const;

		Vector3& operator= (const Vector3& other);
		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);
		float Length() const;
		

		//Measures How similar the direction of a and b are.
		static float Dot(const Vector3& a, const Vector3& b);

		//Returns a vector perpendicular to both a and b
		//I use right-handed coordinate system
		static Vector3 Cross(const Vector3& a, const Vector3& b);

		static bool NearyEquals(const Vector3& a, const Vector3& b);
		Vector3 Normalized() const;

		//교점 계산 함수
		//이걸 통해 어떤 정점이 nearPlane에 가까운지 계산하고 자를것인가 둘것인가 결정
		static Vector3 IntersectNearPlane(
			const Vector3& inside,
			const Vector3& outside,
			float nearPlane
		);

		bool operator == (const Vector3& other) const;
		bool operator != (const Vector3& other) const;

		float x;
		float y;
		float z;
	};
}

