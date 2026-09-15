#include "CollisionSystem.h"
#include "GameObject/GameObject.h"
#include "Component/BoxColliderComponent.h"
#include "Component/BoxCollider3DComponent.h"
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
	bool CollisionSystem::CanMoveTo(
		const GameObject& movingObject, 
		const Vector3& nextPosition, 
		const std::vector<std::shared_ptr<GameObject>>& gameObjectList) const
	{
		const std::shared_ptr<BoxCollider3DComponent> movingCollider =
			movingObject.GetComponent<BoxCollider3DComponent>();

		if (movingCollider == nullptr) return false;


		for (const std::shared_ptr<GameObject>& other : gameObjectList)
		{
			//없는 객체거나 자기자신 그리고 비활성객체는 제외
			if (other == nullptr || other.get() == &movingObject || !other->IsActive()) continue;

			const std::shared_ptr<BoxCollider3DComponent> otherCollider = other->GetComponent<BoxCollider3DComponent>();
			
			if (otherCollider == nullptr) continue;

			//이동예정위치에 상대와 겹치는경우
			if (movingCollider->Intersects(
				nextPosition,
				*otherCollider,
				other->GetWorldPosition()))
			{
				return false;//충돌처리
			}

		}
		//어느 콜라이더와도 충돌하지 않으면움직일 수 있음./
		return true;
	}
	bool CollisionSystem::Test(const std::shared_ptr<GameObject>& left, const std::shared_ptr<GameObject>& right)
	{
		if (left == nullptr || left->IsActive() == false || right == nullptr || right->IsActive() == false) return false;

		std::shared_ptr<BoxColliderComponent> leftCollision = left->GetComponent<BoxColliderComponent>();
		std::shared_ptr<BoxColliderComponent> rightCollision = right->GetComponent<BoxColliderComponent>();


		if (leftCollision == nullptr || rightCollision == nullptr) return false;


		const Vector3 leftCurrent = left->GetWorldPosition();
		const Vector3 leftPrevious = left->GetPreviousPosition();

		const Vector3 rightCurrent = right->GetWorldPosition();
		const Vector3 rightPrevious = right->GetPreviousPosition();


		const float  leftXMin = (leftCurrent.x < leftPrevious.x) ? leftCurrent.x : leftPrevious.x;
		const float  leftXMaxCurrent = leftCurrent.x + leftCollision->GetWidth() - 1;
		const float  leftXMaxPrevious = leftPrevious.x + leftCollision->GetWidth() - 1;
		const float  leftXMax = (leftXMaxCurrent > leftXMaxPrevious) ? leftXMaxCurrent : leftXMaxPrevious;
		
		const float  rightXMin = (rightCurrent.x < rightPrevious.x) ? rightCurrent.x : rightPrevious.x;
		const float  rightXMaxCurrent = rightCurrent.x + rightCollision->GetWidth() - 1;
		const float  rightXMaxPrevious = rightPrevious.x + rightCollision->GetWidth() - 1;
		const float  rightXMax = (rightXMaxCurrent > rightXMaxPrevious) ? rightXMaxCurrent : rightXMaxPrevious;

		if (rightXMin > leftXMax)
		{
			return false;
		}

		if (rightXMax < leftXMin)
		{
			return false;
		}

		return true;
	}
}