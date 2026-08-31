#pragma once
#include "Vector3.h"

class AABBCollider
{
public:
	AABBCollider(
		float minX,
		float maxX,
		float minZ,
		float maxZ
	) : minX(minX), maxX(maxX), minZ(minZ), maxZ(maxZ){}

	bool IsOverlapping(const AABBCollider& other) const;
	
	//º®¹Ì²ô·¯Áü ÇÔ¼ö
	static Vector3 MoveWithWallSlide(
		const Vector3& current,
		const Vector3& movement,
		float halfX,
		float halfZ,
		const AABBCollider& wall);

private:
	float minX;
	float maxX;
	float minZ;
	float maxZ;
};

