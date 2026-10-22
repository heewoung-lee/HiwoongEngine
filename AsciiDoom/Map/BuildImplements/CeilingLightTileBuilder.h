#pragma once

#include "Map/ITileBuilder.h"
#include "Render/SpotLight.h"
#include <memory>

namespace Hiwoong
{
    struct ModelData;

    class CeilingLightTileBuilder : public ITileBuilder
    {
    public:
        explicit CeilingLightTileBuilder(
            const SpotLight& light,
            const std::shared_ptr<const ModelData>& model
        );

        void Build(
            DoomMap& map,
            const Vector3& worldPosition
        ) override;

    private:
        SpotLight light;
        std::shared_ptr<const ModelData> model;
    };
}