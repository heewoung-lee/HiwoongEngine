#include "CeilingLightTileBuilder.h"
#include "Map/DoomMap.h"
#include "ModelTileBuilder.h"
#include <cassert>
namespace Hiwoong
{
    CeilingLightTileBuilder::CeilingLightTileBuilder(
        const SpotLight& light,
        const std::shared_ptr<const ModelData>& model
    )
        : light(light),
        model(model)
    {
        assert(this->model != nullptr);
    }

    void CeilingLightTileBuilder::Build(
        DoomMap& map,
        const Vector3& worldPosition
    )
    {
        // 배치할 등기구모델과 바닥에서 올릴높이를 결정해 위치를 설정
        ModelTileBuilder modelBuilder(model,static_cast<float>(map.GetWallLayers()));

        modelBuilder.Build(map, worldPosition);

        SpotLight placedLight = light;
        placedLight.position = worldPosition;

        placedLight.position.y +=
            map.GetFloorHeight() -
            static_cast<float>(map.GetWallLayers());

        map.AddSpotLight(placedLight);
    }
}