#include "Vector3.h"
#include <cmath>
Vector3::Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

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

Vector3 Vector3::Cross(const Vector3& other) const
{
	float resultX = y * other.z - z * other.y;
	float resultY = z * other.x - x * other.z;
	float resultZ = x * other.y - y * other.x;

	return Vector3(resultX, resultY, resultZ);
}

Vector3 Vector3::FromYaw(float yawRadians)
{
	float sinYaw = std::sin(yawRadians);
	float cosYaw = std::cos(yawRadians);

	return Vector3(sinYaw, 0, cosYaw);
}

bool Vector3::NearlyEquals(const Vector3& other, float epsilon) const
{
	float diffenceX = std::abs(x - other.x);
	float diffenceY = std::abs(y - other.y);
	float diffenceZ = std::abs(z - other.z);

	return 
		diffenceX < epsilon && 
		diffenceY < epsilon &&
		diffenceZ < epsilon 
		;
}

bool Vector3::operator==(const Vector3& other) const
{
	return x == other.x && y == other.y && z == other.z;
}
