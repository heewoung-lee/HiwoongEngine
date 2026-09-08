#pragma once

#include "Component/Component.h"
#include "Math/Vector3.h"

namespace Hiwoong
{
	class Hiwoong_API BoxCollider3DComponent : public Component
	{
		TYPE_DECALRATIONS(BoxCollider3DComponent,Component)

	public:

		explicit BoxCollider3DComponent(const Vector3& halfExtents) : halfExtents(halfExtents) {}

		const Vector3& GetHalfExtents() const
		{
			return halfExtents;
		}

		//충돌 확인
		bool Intersects(
			const Vector3& position,
			const BoxCollider3DComponent& other,
			const Vector3& otherPosition
		) const;

	private:
		Vector3 halfExtents;

	};
}
