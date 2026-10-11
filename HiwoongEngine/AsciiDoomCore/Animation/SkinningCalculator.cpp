#include "SkinningCalculator.h"
#include <cassert>

namespace Hiwoong
{
	
	std::vector<ModelVertex> SkinningCalculator::Calculate(
		const ModelMesh& mesh,
		const std::vector<ModelMatrix>& nodeMatrices,
		const ModelMatrix& modelAlignment
	)
	{
		// 원본을 보존하고, 복사본에 계산 결과를 넣는다
		std::vector<ModelVertex> result = mesh.vertices;

		assert(mesh.localVertices.size() == result.size());
		assert(mesh.skinWeights.size() == result.size());

		for (std::size_t index = 0; index < result.size(); ++index)
		{
			const std::vector<SkinWeight>& weights =
				mesh.skinWeights[index];

			// 영향을 주는 관절이 없으면 기존 위치를 유지한다
			if (weights.empty())
				continue;

			// 정점마다 합산할 위치와 영향 비율을 초기화
			ModelVertex blendedVertex; // x, y, z 기본값은 0
			float totalWeight = 0.0f;

			for (const SkinWeight& weight : weights)
			{
				if (weight.weight <= 0.0f)
					continue;

				const int bindingIdx =
					FindBindingIdx(mesh.skinBindings, weight.nodeIdx);

				if (weight.nodeIdx < 0 ||
					weight.nodeIdx >= static_cast<int>(nodeMatrices.size()) ||
					bindingIdx == -1)
				{
					assert(false && "Invalid skin binding");
					return {};
				}

				// 원본 정점 위치를 관절 기준으로 바꾼다
				const ModelVertex boneVertex =
					mesh.skinBindings[bindingIdx].geometryToBone.TransformPoint(
						mesh.localVertices[index]
					);

				// 현재 관절의 위치와 회전을 적용한다
				const ModelVertex movedVertex =
					nodeMatrices[weight.nodeIdx].TransformPoint(boneVertex);


				blendedVertex.x += movedVertex.x * weight.weight;
				blendedVertex.y += movedVertex.y * weight.weight;
				blendedVertex.z += movedVertex.z * weight.weight;

				totalWeight += weight.weight;
			}
			// 실제로 영향을 준 관절이 없으면 기존 위치를 유지한다.
			if (totalWeight <= 0.0f)
				continue;

			// 기존 메시와 같은 중심 기준으로 맞춘다.
			const ModelVertex alignedVertex =
				modelAlignment.TransformPoint(blendedVertex);

			// 위치만 바꾸므로 기존 UV는 유지된다.
			result[index].x = alignedVertex.x;
			result[index].y = alignedVertex.y;
			result[index].z = alignedVertex.z;
		}

		return result;
	}
	int SkinningCalculator::FindBindingIdx(
		const std::vector<SkinBinding>& bindings,
		int nodeIdx
	)
	{
		for (int index = 0; index < static_cast<int>(bindings.size()); ++index)
		{
			if (bindings[index].nodeIdx == nodeIdx)
				return index;
		}

		return -1;
	}
}