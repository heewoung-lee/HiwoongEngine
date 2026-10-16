#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
    class RestartButton : public GameObject
    {
        TYPE_DECALRATIONS(RestartButton, GameObject)

    public:
        void Start() override;
    };
}