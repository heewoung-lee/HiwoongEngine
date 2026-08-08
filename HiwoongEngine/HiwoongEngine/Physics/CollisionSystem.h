#pragma once

#include <memory>
#include <vector>

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

	private:
		bool Test(const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right);

	};

}
