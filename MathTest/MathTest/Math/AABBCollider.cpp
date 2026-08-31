#include "AABBCollider.h"

bool AABBCollider::IsOverlapping(const AABBCollider& other) const
{
	bool isOverX = minX <= other.maxX && maxX >= other.minX;
	bool isOverZ = minZ <= other.maxZ && maxZ >= other.minZ;

	return isOverX && isOverZ;
}

Vector3 AABBCollider::MoveWithWallSlide(const Vector3& current, const Vector3& movement, float halfX, float halfZ, const AABBCollider& wall)
{
	Vector3 result = current;

	Vector3 xCandidate
	(
		current.GetX() + movement.GetX(),
		current.GetY(),
		current.GetZ()
	);

	AABBCollider xCollider(
		xCandidate.GetX() - halfX,
		xCandidate.GetX() + halfX,
		xCandidate.GetZ() - halfZ,
		xCandidate.GetZ() + halfZ
	);

	//만약 충돌하지 않으면 result 갱신
	if (xCollider.IsOverlapping(wall) == false) result = xCandidate;

	Vector3 zCandidate
	(
		result.GetX(),
		result.GetY(),
		result.GetZ() + movement.GetZ()
	);

	AABBCollider zCollider(
		zCandidate.GetX() - halfX,
		zCandidate.GetX() + halfX,
		zCandidate.GetZ() - halfZ,
		zCandidate.GetZ() + halfZ
	);

	//만약 충돌하지 않으면 result 갱신
	if (zCollider.IsOverlapping(wall) == false) result = zCandidate;


	return result;
}
