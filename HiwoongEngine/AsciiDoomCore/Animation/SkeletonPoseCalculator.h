#pragma once

#include "../ModelImport/ModelMatrix.h"
#include "../ModelImport/SkeletonNode.h"
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
		static std::vector<ModelMatrix> Calculate(
			const std::vector<SkeletonNode>& nodes
		);
	};
}