#pragma once

#include "Component/Component.h"
#include "PathFinder/GridPosition.h"
#include "Math/Vector3.h"
#include <vector>
#include <memory>
#include <cstddef>
namespace Hiwoong
{
	class IPathFinder;
	class INavigationMap;
	class GameObject;
	class Scene;
	class TransformComponent;

	class NavAIComponent : public Component
	{
		TYPE_DECALRATIONS(NavAIComponent,Component)

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="speed">이동속도</param>
		/// <param name="stopDistance">이거리까지 접근하면 정지</param>
		/// <param name="repathInterval">경로를 재탐색하는데 필요한 최소간격</param>
		NavAIComponent(
			float speed,
			float stopDistance,
			float repathInterval = 0.25f);

		void Start() override;
		void Update(double deltaTime) override;

	private:
		void InitReferences();
		void RequestPath();
		bool IsWithinStopDistance(
			const Vector3& currentPosition,
			const Vector3& targetPosition
		) const;
		void FollowPath(double deltaTime);

		float speed;
		float stopDistance;
		float repathInterval;
		float repathElapsedTime;

		std::shared_ptr<const IPathFinder> pathFinder;

		std::weak_ptr<const INavigationMap> navigationMap;
		std::weak_ptr<GameObject> target;
		std::weak_ptr<Scene> scene;
		std::weak_ptr<TransformComponent> transform;

		std::vector<GridPosition> path;
		std::size_t currentPathIndex = 0;

	};
}



