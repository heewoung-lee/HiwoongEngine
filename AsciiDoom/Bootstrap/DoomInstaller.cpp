#include "DoomInstaller.h"
#include "PathFinder/Implements/AStarPathFinder.h"

namespace Hiwoong
{
	std::shared_ptr<const IPathFinder> DoomInstaller::CreatePathFinder()
	{
		//AStarPathFinder 구현체 의존성 주입
		return std::make_shared<AStarPathFinder>();
	}
}
