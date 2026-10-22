#pragma once

#include "GameObject/GameObject.h"
#include "Component/BoxCollider3DComponent.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace Hiwoong
{
    struct ModelData;

    class MapPropObject : public GameObject
    {
        TYPE_DECALRATIONS(MapPropObject, GameObject)

    public:
        MapPropObject(
            const Vector3& position,
            const std::shared_ptr<const ModelData>& model
        );

        void Start() override;

    private:
        //모델을 표시하는 초기화구문.
        void InitMeshRenderer();
        //모델 크기에 맞는 콜라이어 추가
        void InitCollider();

    private:
        std::shared_ptr<const ModelData> model;
    };
}