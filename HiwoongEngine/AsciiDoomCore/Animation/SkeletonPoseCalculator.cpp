#include "SkeletonPoseCalculator.h"
#include <cassert>
#include <cstddef>

namespace Hiwoong
{
	std::vector<ModelMatrix> SkeletonPoseCalculator::Calculate(
		const std::vector<SkeletonNode>& nodes
	)
	{
		std::vector<ModelMatrix> matrices;

		for (const SkeletonNode& node : nodes)
		{
			ModelMatrix matrix = ModelMatrix::FromNode(node);
			int parentIdx = node.parentIdx;
			std::size_t parentCount = 0;

			while (parentIdx != -1)
			{
				++parentCount;

				// 잘못된 부모 번호나 순환하는 연결을 검사.
				if (parentIdx < 0 ||
					parentIdx >= static_cast<int>(nodes.size()) ||
					parentCount >= nodes.size())
				{
					assert(false && "Invalid skeleton hierarchy");
					return {};
				}

				const SkeletonNode& parent = nodes[parentIdx];

				matrix = ModelMatrix::FromNode(parent).Multiply(matrix);
				parentIdx = parent.parentIdx;
			}

			matrices.push_back(matrix);
		}

		return matrices;
	}
}