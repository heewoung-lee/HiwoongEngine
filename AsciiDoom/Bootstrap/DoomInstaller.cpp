#include "DoomInstaller.h"
#include "PathFinder/Implements/AStarPathFinder.h"
#include "ModelImport/ModelImporter.h"
#include "ModelImport/UfbxModelSourceReader.h"
#include "ModelImport/MeshLoader.h"
#include "ModelImport/SkeletonLoader.h"
#include "ModelImport/SkinWeightLoader.h"
namespace Hiwoong
{
	std::shared_ptr<const IPathFinder> DoomInstaller::CreatePathFinder()
	{
		//AStarPathFinder 구현체 의존성 주입
		return std::make_shared<AStarPathFinder>();
	}
	std::shared_ptr<const IModelImporter>
		DoomInstaller::CreateModelImporter()
	{
		return std::make_shared<ModelImporter>(
			std::make_shared<UfbxModelSourceReader>(),
			std::make_shared<MeshLoader>(),
			std::make_shared<SkeletonLoader>(),
			std::make_shared<SkinWeightLoader>()
		);
	}
}
