#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
    class MenuCursor : public GameObject
    {
        TYPE_DECALRATIONS(MenuCursor, GameObject)

    public:
        void Start() override;

        void SetTarget(
            const std::shared_ptr<GameObject>& target
        );
    };
}