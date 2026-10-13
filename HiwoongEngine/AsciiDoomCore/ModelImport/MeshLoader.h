#pragma once

#include "IMeshLoader.h"
#include "ModelImportContext.h"
#include "MeshLoadResult.h"

namespace Hiwoong
{
	class MeshLoader final : public IMeshLoader
	{
	public:
		MeshLoadResult Load(
			const ModelImportContext& context
		)const override;
	};
}