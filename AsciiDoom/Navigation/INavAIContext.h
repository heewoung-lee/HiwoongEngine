#pragma once

#include <memory>


namespace Hiwoong
{
	class IPathFinder;
	class INavigationMap;
	class GameObject;

	class INavAIContext
	{
	public:
		virtual ~INavAIContext() = default;

		//길찾기 안내서
		virtual std::shared_ptr<const IPathFinder> GetPathFinder() const = 0;
		
		//맵정보
		virtual std::shared_ptr<const INavigationMap> GetNavigationMap() const = 0;

		//타겟
		virtual std::shared_ptr<GameObject> GetNavigationTarget() const = 0;
	};


}
