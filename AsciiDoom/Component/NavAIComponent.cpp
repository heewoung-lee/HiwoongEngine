#include "NavAIComponent.h"
#include "Navigation/INavAIContext.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"
#include "Component/TransformComponent.h"
#include "Navigation/INavigationMap.h"
#include "PathFinder/Interfaces/IPathFinder.h"
#include <cassert>

namespace Hiwoong
{

	NavAIComponent::NavAIComponent(
		float speed,
		float stopDistance,
		float repathInterval
	)
		: speed(speed),
		stopDistance(stopDistance),
		repathInterval(repathInterval),
		repathElapsedTime(repathInterval) //바로 넣어서 처음부터 길찾기를 할 수 있도록
	{
	}

	void NavAIComponent::Start()
	{
		super::Start();
		InitReferences();
	}


	void NavAIComponent::Update(double deltaTime)
	{
		super::Update(deltaTime);

		const auto targetObject = target.lock();
		const auto ownerTransform = transform.lock();

		if (targetObject == nullptr || ownerTransform == nullptr)
			return;

		if (targetObject->IsActive() == false)
			return;

		//가까워지면 종료
		if (IsWithinStopDistance(
			ownerTransform->GetWorldPosition(),
			targetObject->GetWorldPosition()))
		{
			return;
		}

		repathElapsedTime += static_cast<float>(deltaTime);

		//경로가 비어 있고 갱신 시간이 됐다면 길을 찾는다.
		if (path.empty() &&
			repathElapsedTime >= repathInterval)
		{
			RequestPath();

		}
		//경로를 따라 이동하고, 칸에 도착했을 때 갱신 시간이 됐다면 새 길을 찾는다.
		FollowPath(deltaTime);
	}

	/// <summary>
	/// 필요한 인스턴스 연결
	/// </summary>
	void NavAIComponent::InitReferences()
	{
		const std::shared_ptr<Scene> currentScene = GetScene();

		const std::shared_ptr<INavAIContext> context =
			std::dynamic_pointer_cast<INavAIContext>(currentScene);

		assert(context != nullptr);
		if (context == nullptr) return;

		scene = currentScene;
		pathFinder = context->GetPathFinder();
		navigationMap = context->GetNavigationMap();
		target = context->GetNavigationTarget();

		transform = GetComponent<TransformComponent>();
	}
	/// <summary>
	/// 내 위치와 대상 위치를 격자로 바꿔 A*에 전달하고, 결과를 path에 저장
	/// </summary>
	void NavAIComponent::RequestPath()
	{
		path.clear();
		repathElapsedTime = 0.0f;
		currentPathIndex = 0;

		const auto map = navigationMap.lock();
		const auto targetObject = target.lock();
		const auto ownerTransform = transform.lock();

		if (map == nullptr ||
			targetObject == nullptr ||
			ownerTransform == nullptr ||
			pathFinder == nullptr)
		{
			return;
		}

		if (targetObject->IsActive() == false) return;

		const GridPosition start =
			map->WorldToGrid(ownerTransform->GetWorldPosition());

		const GridPosition destination =
			map->WorldToGrid(targetObject->GetWorldPosition());

		path = pathFinder->FindPath(
			map->GetNavigationGrid(),
			start,
			destination
		);
	}

	bool NavAIComponent::IsWithinStopDistance(
		const Vector3& currentPosition,
		const Vector3& targetPosition
	) const
	{
		Vector3 difference = targetPosition - currentPosition;
		difference.y = 0.0f;

		return difference.Length() <= stopDistance;
	}

	void NavAIComponent::FollowPath(double deltaTime)
	{
		if (currentPathIndex >= path.size()) return;

		const auto map = navigationMap.lock();
		const auto ownerTransform = transform.lock();

		if (map == nullptr || ownerTransform == nullptr) return;

		const Vector3 currentPosition =
			ownerTransform->GetWorldPosition();

		const Vector3 waypoint =
			map->GridToWorld(path[currentPathIndex]);

		Vector3 direction = waypoint - currentPosition;
		direction.y = 0.0f;

		const float distance = direction.Length();

		if (distance <= 0.01f)
		{
			++currentPathIndex;

			if (repathElapsedTime >= repathInterval)
			{
				RequestPath();
			}
			else if (currentPathIndex >= path.size())
			{
				//기존 경로를 끝까지 따라왔으므로 비운다.
				//Update에서 갱신 시간이 되면 다시 길을 찾는다.
				path.clear();
			}

			return;
		}

		const Vector3 moveDirection = direction.Normalized();

		float remainingDistance =
			speed * static_cast<float>(deltaTime);

		if (remainingDistance > distance)
		{
			remainingDistance = distance;
		}


		const Vector3 nextPosition = currentPosition + moveDirection * remainingDistance;

		//이동해도 되는지 물어봐야 하기 때문에 씬을 호출
		const auto currentScene = scene.lock();

		//현재 이 컴포넌트가 붙어있는 오브젝트를 움직여야함.
		const auto ownerObject = GetOwner();
		assert(currentScene != nullptr);
		assert(ownerObject != nullptr);

		if (currentScene->CanMoveTo(*ownerObject, nextPosition) == false)
		{
			return;
		}

		//
		ownerTransform->SetWorldPosition(nextPosition);
	}

}

