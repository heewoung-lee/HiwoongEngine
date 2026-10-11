#pragma once

#include "ISkeletonLoader.h"

namespace Hiwoong
{
	class SkeletonLoader final : public ISkeletonLoader
	{
	public:
		std::vector<SkeletonNode> Load(const ModelImportContext& context) const override;
	};
}
