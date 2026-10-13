#pragma once

#include "SkeletonNode.h"
#include <vector>

namespace Hiwoong
{
	struct ModelImportContext;
	/// <summary>
	/// 스켈레톤 정보를 로드하는 인터페이스
	/// </summary>
	class ISkeletonLoader
	{
	public:
		virtual ~ISkeletonLoader() = default;

		virtual std::vector<SkeletonNode> Load(const ModelImportContext& context) const = 0;
	};


}
