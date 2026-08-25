#pragma once
#include <cmath>

class Vector3
{
public:
	Vector3(float x, float y, float z);
	~Vector3();


	inline  Vector3 GetPosition() const { return Vector3(x, y, z); }

	float GetLength() const;
	Vector3 Normalized() const;
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	static Vector3 FromYaw(float yawRadians);

	bool NearlyEquals(const Vector3& other, float epsilon) const;

	bool operator == (const Vector3& other) const;
		
private:

	float x;
	float y;
	float z;
};

