#include "BoxCollider3DComponent.h"
#include <cmath>
namespace Hiwoong
{
	bool BoxCollider3DComponent::Intersects(const Vector3& position, const BoxCollider3DComponent& other, const Vector3& otherPosition) const
	{
		const Vector3 distance = otherPosition - position;
		const Vector3 combineHalfExtents = halfExtents + other.GetHalfExtents();

		
		//1) 각 BoxCollider3DComponent의 중심거리를 구한다.
		//2) 반쪽 크기가 각각 0.5, 0.2라면 합은 0.7.
		//3) 즉 중심거리가 > 0.7 이면 그 축에서 떨어져 있는거.
		//4) 그걸 x,y,z,전부 순회하면서 확인하고 하나라도 떨어져있다면 false로 리턴.
		if (std::abs(distance.x) > combineHalfExtents.x) return false;
		if (std::abs(distance.y) > combineHalfExtents.y) return false;
		if (std::abs(distance.z) > combineHalfExtents.z) return false;

		//세축 모두 겹치면 충돌
		return true;

	}

}

