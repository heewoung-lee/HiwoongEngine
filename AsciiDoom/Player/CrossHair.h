#pragma once
#include "GameObject/GameObject.h"
#include "Math/Vector2.h"
namespace Hiwoong
{
    class Crosshair : public GameObject
    {
        TYPE_DECALRATIONS(Crosshair, GameObject)

    public:
        Crosshair();
        void Start() override;
        Vector2 GetScreenCenter() const;
    };
}