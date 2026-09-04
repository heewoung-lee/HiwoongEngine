#pragma once

#include "Component/Component.h"
#include "Math/Vector3.h"
#include "Component/TransformComponent.h"

namespace Hiwoong
{
    class TransformComponent;

    class CubeRotationComponent : public Component
    {
        TYPE_DECALRATIONS(CubeRotationComponent, Component)

    public:
        void Start() override;
        void Update(double deltaTime) override;

    private:
        std::weak_ptr<TransformComponent> transform3DComponent;
        Vector3 rotationSpeed{ 0.5f, 0.8f, 0.0f };
    };
}