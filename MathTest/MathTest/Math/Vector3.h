#pragma once
#include <cmath>

class Vector3
{
public:
	Vector3(float x, float y, float z);
	~Vector3();


	inline  Vector3 GetPosition() const { return Vector3(x, y, z); }
	inline  float GetX() const { return x; }
	inline  float GetY() const { return y; }
	inline  float GetZ() const { return z; }

	static Vector3 FromYaw(float yawRadians);
	static Vector3 GetProjection(const Vector3& cameraPosition);
	static Vector3 IntersectNearPlane(
		const Vector3& a,
		const Vector3& b,
		float nearPlane);

	//
	static float RayPlaneIntersectionT
	(const Vector3& origin,
		const Vector3& direction,
		const Vector3& planePoint,
		const Vector3& planeNormal);


	float GetLength() const;
	Vector3 Normalized() const;
	float Dot(const Vector3& other) const;
	Vector3 Cross(const Vector3& other) const;
	bool NearlyEquals(const Vector3& other, float epsilon) const;
	


	bool operator == (const Vector3& other) const;
	Vector3 operator -(const Vector3& other) const;
	Vector3 operator *(float value) const;
	Vector3 operator +(const Vector3& other) const;

	static Vector3 GetPointOnRay(
		const Vector3& origin,
		const Vector3& direction,
		float t);



private:

	float x;
	float y;
	float z;
};

