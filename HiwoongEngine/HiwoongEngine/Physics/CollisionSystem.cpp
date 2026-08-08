#include "CollisionSystem.h"
#include "GameObject/GameObject.h"
#include "Component/BoxColliderComponent.h"
#include <vector>

namespace Hiwoong
{
	void CollisionSystem::ProcessCollision(const std::vector<std::shared_ptr<GameObject>>& gameObjectList)
	{
		if (gameObjectList.empty()) return;

		std::vector<CollisionPair> collidedObjectList;

		const int count = static_cast<int>(gameObjectList.size());

		for (int ix = 0; ix < count; ++ix)
		{
			const std::shared_ptr<GameObject>& left = gameObjectList[ix];

			if (left == nullptr || left->IsActive()) continue;

			for (int jx = ix + 1; jx < count;++jx)
			{
				const std::shared_ptr<GameObject>& right = gameObjectList[jx];
				if (right == nullptr || right->IsActive()) continue;

				if (Test(left, right))
				{
					CollisionPair pair = {};
					pair.gameObject = left;
					pair.other = right;

					collidedObjectList.emplace_back(pair);
				}
			}
		}


		if (collidedObjectList.empty()) return;


		for (const CollisionPair& pair : collidedObjectList)
		{
			if (pair.gameObject->IsActive() == false || pair.other->IsActive() == false) continue;

			pair.gameObject->OnCollision(pair.other);
			pair.other->OnCollision(pair.gameObject);
		}

	}
	bool CollisionSystem::Test(const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right)
	{
		if (left == nullptr || left->IsActive() == false || right == nullptr || right->IsActive() == false) return false;

		std::shared_ptr<BoxColliderComponent> leftCollision = left->GetComponent<BoxColliderComponent>();
		std::shared_ptr<BoxColliderComponent> rightCollision = right->GetComponent<BoxColliderComponent>();


		if (leftCollision == nullptr || rightCollision == nullptr) return false;


		const Vector2 leftCurrent = left->GetWorldPosition();
		const Vector2 leftPrevious = left->GetPreviousPosition();

		const Vector2 rightCurrent = right->GetWorldPosition();
		const Vector2 rightPrevious = right->GetPreviousPosition();


		const int leftXMin = (leftCurrent.x < leftPrevious.x) ? leftCurrent.x : leftPrevious.x;
		const int leftXMaxCurrent = leftCurrent.x + leftCollision->GetWidth() - 1;
		const int leftXMaxPrevious = leftPrevious.x + leftCollision->GetWidth() - 1;
		const int leftXMax = (leftXMaxCurrent > leftXMaxPrevious) ? leftXMaxCurrent : leftXMaxPrevious;

		const int rightXMin = (rightCurrent.x < rightPrevious.x) ? rightCurrent.x : rightPrevious.x;
		const int rightXMaxCurrent = rightCurrent.x + rightCollision->GetWidth() - 1;
		const int rightXMaxPrevious = rightPrevious.x + rightCollision->GetWidth() - 1;
		const int rightXMax = (rightXMaxCurrent > rightXMaxPrevious) ? rightXMaxCurrent : rightXMaxPrevious;

		if (rightXMin > leftXMax)
		{
			return false;
		}

		if (rightXMax < leftXMin)
		{
			return false;
		}

	}
}