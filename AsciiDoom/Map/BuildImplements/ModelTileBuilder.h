#pragma once

#include "Map/ITileBuilder.h"
#include <memory>

namespace Hiwoong
{
    struct ModelData;

    class ModelTileBuilder : public ITileBuilder
    {
    public:
        explicit ModelTileBuilder(
            const std::shared_ptr<const ModelData>& model,
            float heightFromFloor = 0.0f
        );

        void Build(
            DoomMap& map,
            const Vector3& worldPosition
        ) override;

    private:
        std::shared_ptr<const ModelData> model;
        float heightFromFloor;
    };
}