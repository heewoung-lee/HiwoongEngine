#pragma once

#include "ModelMesh.h"
#include <vector>

namespace Hiwoong
{
	struct ModelImportContext;
	/// <summary>
	/// 원본 가충지를 출력 정점에 맞춰 옮기는 계약.
	/// </summary>
	class ISkinWeightLoader
	{
	public:
		virtual ~ISkinWeightLoader() = default;

		virtual bool Load(
			const ModelImportContext& context,
			std::vector<ModelMesh>& meshes
		) const = 0;
	};
}