#pragma once

#include "Component/Component.h"
#include "Animation/SpriteAnimationClip.h"

namespace Hiwoong
{
    class ScreenEffectObject;
    class Camera3D;

    class PlayerHitEffectComponent : public Component
    {
        TYPE_DECALRATIONS(PlayerHitEffectComponent, Component)
    public:
        explicit PlayerHitEffectComponent(
            const SpriteAnimationClip& clip,
            float shakeStrength,
            double shakeDuration
        );
        void Start() override;
        void Update(double deltaTime) override;
    private:
        void OnHpChanged(int hp);

        SpriteAnimationClip clip;
        std::weak_ptr<ScreenEffectObject> screenEffect;
        int previousHp = 0;

        //카메라 쉐이크용
        Camera3D* camera = nullptr;

        float shakeStrength;
        double shakeDuration;
    };
}