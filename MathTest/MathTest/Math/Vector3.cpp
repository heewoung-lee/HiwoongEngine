#include "Vector3.h"

Vector3::Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y),z(z){}

Vector3::~Vector3()
{
}

float Vector3::GetLength() const
{
	return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized() const
{
	float length = GetLength();
	if (length == 0) return Vector3(0, 0, 0);

	float normalX = x / length;
	float normalY = y / length;
	float normalZ = z / length;
	return Vector3(normalX, normalY, normalZ);
}

float Vector3::Dot(const Vector3& other) const
{
	return (x * other.x + y * other.y + z * other.z);
}
