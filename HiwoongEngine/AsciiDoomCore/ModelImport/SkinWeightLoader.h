#pragma once

#include "ISkinWeightLoader.h"

namespace Hiwoong
{
    class SkinWeightLoader final : public ISkinWeightLoader
    {
    public:
        bool Load(
            const ModelImportContext& context,
            std::vector<ModelMesh>& meshes
        ) const override;
    };
}