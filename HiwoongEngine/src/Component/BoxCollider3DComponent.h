#pragma once

#include "Component/Component.h"
#include "Math/Vector3.h"

namespace Hiwoong
{
	class Hiwoong_API BoxCollider3DComponent : public Component
	{
		TYPE_DECALRATIONS(BoxCollider3DComponent,Component)

	public:

		explicit BoxCollider3DComponent(
			const Vector3& halfExtents,
			bool isTrigger = false
		)
			: halfExtents(halfExtents),
			isTrigger(isTrigger)
		{
		}


		const Vector3& GetHalfExtents() const
		{
			return halfExtents;
		}

		bool IsTrigger() const
		{
			return isTrigger;
		}

		//충돌 확인
		bool Intersects(
			const Vector3& position,
			const BoxCollider3DComponent& other,
			const Vector3& otherPosition
		) const;



	private:
		//10.14일 트리거 추가, 이전에는 OnCollision 기능 밖에 없었는데
		// 트리거 충돌이 필요해서 넣음.
		bool isTrigger;
		Vector3 halfExtents;

	};
}
