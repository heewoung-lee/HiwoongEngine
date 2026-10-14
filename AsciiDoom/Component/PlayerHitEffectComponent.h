#pragma once

#include "Component/Component.h"
#include "Animation/SpriteAnimationClip.h"

namespace Hiwoong
{
    class ScreenEffectObject;

    class PlayerHitEffectComponent : public Component
    {
        TYPE_DECALRATIONS(PlayerHitEffectComponent, Component)

    public:
        explicit PlayerHitEffectComponent(
            const SpriteAnimationClip& clip
        );

        void Start() override;

    private:
        void OnHpChanged(int hp);

        SpriteAnimationClip clip;
        std::weak_ptr<ScreenEffectObject> screenEffect;
        int previousHp = 0;
    };
}