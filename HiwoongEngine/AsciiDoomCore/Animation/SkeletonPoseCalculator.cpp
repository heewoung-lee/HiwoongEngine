#include "SkeletonPoseCalculator.h"
#include <cassert>
#include <cstddef>

namespace Hiwoong
{
	std::vector<Matrix4x4> SkeletonPoseCalculator::Calculate(
		const std::vector<SkeletonNode>& nodes
	)
	{
		std::vector<Matrix4x4> matrices;

		for (const SkeletonNode& node : nodes)
		{
			Matrix4x4 matrix = CreateLocalMatrix(node);
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

				matrix = CreateLocalMatrix(parent) * matrix;
				parentIdx = parent.parentIdx;
			}

			matrices.push_back(matrix);
		}

		return matrices;
	}
	std::vector<Matrix4x4> SkeletonPoseCalculator::Calculate(
		const std::vector<SkeletonNode>& modelNodes,
		const AnimationClip& clip,
		double time
	)
	{
		std::vector<Matrix4x4> matrices = Calculate(modelNodes);
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

		const std::vector<Matrix4x4> animationMatrices =
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
	Matrix4x4 SkeletonPoseCalculator::CreateLocalMatrix(
		const SkeletonNode& node
	)
	{
		const Matrix4x4 translation = Matrix4x4::Translation(
			Vector3(
				node.localPositionX,
				node.localPositionY,
				node.localPositionZ
			)
		);

		const Matrix4x4 rotation = Matrix4x4::RotationQuaternion(
			node.localRotationX,
			node.localRotationY,
			node.localRotationZ,
			node.localRotationW
		);

		const Matrix4x4 scale = Matrix4x4::Scale(
			Vector3(
				node.localScaleX,
				node.localScaleY,
				node.localScaleZ
			)
		);

		return translation * rotation * scale;
	}
}