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
	void NavAIComponent::Start()
	{
		super::Start();
		InitReferences();
	}
	void NavAIComponent::InitReferences()
	{
		const auto currentScene = GetScene();

		const auto context =
			std::dynamic_pointer_cast<INavAIContext>(currentScene);

		assert(context != nullptr);
		if (context == nullptr) return;

		pathFinder = context->GetPathFinder();
		navigationMap = context->GetNavigationMap();

		assert(pathFinder != nullptr);
		assert(!navigationMap.expired());
	}

	std::vector<Vector3> NavAIComponent::FindPath(
		const Vector3& startPosition,
		const Vector3& targetPosition
	) const
	{
		const auto map = navigationMap.lock();

		assert(map != nullptr);
		assert(pathFinder != nullptr);

		if (map == nullptr || pathFinder == nullptr)
			return {};

		const auto gridPath = pathFinder->FindPath(
			map->GetNavigationGrid(),
			map->WorldToGrid(startPosition),
			map->WorldToGrid(targetPosition)
		);

		std::vector<Vector3> worldPath;

		for (const GridPosition& position : gridPath)
		{
			worldPath.push_back(map->GridToWorld(position));
		}

		return worldPath;
	}
}

