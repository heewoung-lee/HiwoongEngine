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

//Z축 이 기본 축이라 모든 축들을 기본축으로 나눠 노멀로 만듦
//즉 카메라가 z로 부터 멀어지면 x,y축도 같은 비율로 줄어들어야함.

Vector3 Vector3::GetProjection(const Vector3& cameraPosition)
{
	Vector3 result;

	result.x = cameraPosition.x / cameraPosition.z;
	result.y = cameraPosition.y / cameraPosition.z;
	result.z = cameraPosition.z;

	return result;
}

//카메라에서 벗어낸 정점을 제거 한 뒤.
//안쪽으로 들어오는 선분을 하나 그어,
//새로운 버택스를 만들어 내어, 버택스가 전부 날라가는걸 방지.
Vector3 Vector3::IntersectNearPlane(const Vector3& inside, const Vector3& outside, float nearPlane)
{
	//t는 A에서 B까지 0~1 중 어디까지 이동하면 Near Plane에 닿는지 나타내는 비율.
	float t = (nearPlane - inside.z) / (outside.z - inside.z);

	// 기존 선분 AB가 Near Plane과 만나는 점 I
	float resultX = inside.x + t * (outside.x - inside.x);
	float resultY = inside.y + t * (outside.y - inside.y);
	float resultZ = inside.z + t * (outside.z - inside.z);

	return Vector3(resultX, resultY, resultZ);
}

//
float Vector3::RayPlaneIntersectionT(const Vector3& origin, const Vector3& direction, const Vector3& planePoint, const Vector3& planeNormal)
{
	return 0.0f;
}
