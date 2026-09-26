#pragma once

#include "PathFinder/NavigationGrid.h"
#include "PathFinder/GridPosition.h"
#include "Math/Vector3.h"
namespace Hiwoong
{
	class INavigationMap
	{
	public:
		virtual ~INavigationMap() = default;

		virtual const NavigationGrid& GetNavigationGrid() const = 0;
		
		virtual GridPosition WorldToGrid(const Vector3& worldPos) const = 0;
		virtual Vector3 GridToWorld(const GridPosition& gridPos) const = 0;
	};
}

