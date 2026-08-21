#pragma once

#include "Component/Component.h"
#include "Math/Vector3.h"

namespace Hiwoong
{
    class CubeRotationComponent : public Component
    {
        TYPE_DECALRATIONS(CubeRotationComponent, Component)

    public:
        void Update(double deltaTime) override;

        const Vector3& GetRotation() const
        {
            return rotation;
        }

    private:
        Vector3 rotation{ 0.4f, 0.6f, 0.0f };
        Vector3 rotationSpeed{ 0.5f, 0.8f, 0.0f };
    };
}