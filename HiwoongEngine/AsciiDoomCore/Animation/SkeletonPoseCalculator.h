#pragma once

#include "../ModelImport/SkeletonNode.h"
#include "Math/Matrix4x4.h"
#include "AnimationClip.h"
#include <vector>

namespace Hiwoong
{
	/// <summary>
	/// 관절 정보를 읽고 읽은 정보로 관절별 행렬을 구하는 역할.
	/// 즉 회전에 부모의 움직임까지 반영해서 최종적으로는 정점들의 새 위치를 구하기 위함.
	/// </summary>
	class SkeletonPoseCalculator
	{
	public:
		static std::vector<Matrix4x4> Calculate(
			const std::vector<SkeletonNode>& nodes
		);

		static std::vector<Matrix4x4> Calculate(
			const std::vector<SkeletonNode>& modelNodes,
			const AnimationClip& clip,
			double time
		);

	private:
		static Matrix4x4 CreateLocalMatrix(const SkeletonNode& node);
	};
}