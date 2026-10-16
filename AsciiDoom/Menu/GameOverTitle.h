#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
    class GameOverTitle : public GameObject
    {
        TYPE_DECALRATIONS(GameOverTitle, GameObject)

    public:
        void Start() override;
    };
}