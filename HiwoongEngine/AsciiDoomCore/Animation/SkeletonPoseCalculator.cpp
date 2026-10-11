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
	std::vector<ModelMatrix> SkeletonPoseCalculator::Calculate(
		const std::vector<SkeletonNode>& modelNodes,
		const AnimationClip& clip,
		double time
	)
	{
		std::vector<ModelMatrix> matrices = Calculate(modelNodes);
		if (clip.frames.empty())
			return matrices;

		std::size_t frameIndex = 0;

		for (std::size_t i = 1; i < clip.frames.size(); ++i)
		{
			if (clip.frames[i].time > time)
				break;

			frameIndex = i;
		}

		const AnimationFrame& frame = clip.frames[frameIndex];

		const std::vector<ModelMatrix> animationMatrices =
			Calculate(frame.nodes);

		if (clip.sourceNodeIndices.size() != matrices.size())
		{
			assert(false && "Invalid animation node mapping");
			return {};
		}

		for (std::size_t i = 0; i < matrices.size(); ++i)
		{
			const int sourceIndex = clip.sourceNodeIndices[i];

			if (sourceIndex == -1)
				continue;

			if (sourceIndex < 0 ||
				sourceIndex >= static_cast<int>(animationMatrices.size()))
			{
				assert(false && "Invalid animation node index");
				return {};
			}

			matrices[i] = animationMatrices[sourceIndex];
		}
		return matrices;
	}
}