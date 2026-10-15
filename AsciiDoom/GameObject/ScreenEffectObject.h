#pragma once

#include "GameObject/GameObject.h"
#include "Animation/SpriteAnimationClip.h"

namespace Hiwoong
{
    class SpriteAnimationComponent;
    class ScreenEffectObject : public GameObject
    {
        TYPE_DECALRATIONS(ScreenEffectObject, GameObject)

    public:
        explicit ScreenEffectObject(const SpriteAnimationClip& clip);

        void Start() override;
        void Play();
    private:
        SpriteAnimationClip clip;
        std::shared_ptr<SpriteAnimationComponent> animation;
    };
}