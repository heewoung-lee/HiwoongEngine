#include "ModelTileBuilder.h"
#include "Map/DoomMap.h"
#include "GameObject/MapPropObject.h"
#include "Scene/Scene.h"
#include <cassert>

namespace Hiwoong
{
    ModelTileBuilder::ModelTileBuilder(
        const std::shared_ptr<const ModelData>& model,
        float heightFromFloor
    )
        : model(model),
        heightFromFloor(heightFromFloor)
    {
        assert(this->model != nullptr);
    }

    void ModelTileBuilder::Build(
        DoomMap& map,
        const Vector3& worldPosition
    )
    {
        Vector3 placementPosition = worldPosition;
        placementPosition.y += map.GetFloorHeight() - heightFromFloor;

        map.Instantiate<MapPropObject>(
            placementPosition,
            model
        );
    }

}