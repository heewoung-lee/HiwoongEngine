#include "SkeletonLoader.h"
#include "ModelImportContext.h"
#include "IModelSource.h"
#include <cassert>


namespace Hiwoong
{
	std::vector<SkeletonNode> SkeletonLoader::Load(const ModelImportContext& context) const
	{
		assert(context.source != nullptr);

		return context.source->GetSkeletonNodes();
	}

}
