#pragma once

#include <memory>
#include <vector>
#include "Math/Vector3.h"
namespace Hiwoong
{
	class GameObject;

	class  CollisionSystem
	{
		struct CollisionPair
		{
			std::shared_ptr<GameObject> gameObject;
			std::shared_ptr<GameObject> other;
		};

	public:
		CollisionSystem() = default;
		~CollisionSystem() = default;

		void ProcessCollision(const std::vector<std::shared_ptr<GameObject>>& gameObjectList);

		//°ãÄ¡¸é false¹ÝÈ¯
		bool CanMoveTo(
			const GameObject& movingObject,
			const Vector3& nextPosition,
			const std::vector<std::shared_ptr<GameObject>>& gameObjectList
		) const;

	private:
		bool Test(const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right);

	};

}
