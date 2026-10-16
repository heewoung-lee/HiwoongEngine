#pragma once

#include "GameObject/GameObject.h"
#include "Animation/SpriteAnimationClip.h"
#include <functional>

namespace Hiwoong
{
    class SpriteAnimationComponent;
    class ScreenEffectObject : public GameObject
    {
        TYPE_DECALRATIONS(ScreenEffectObject, GameObject)

    public:
        explicit ScreenEffectObject(
            const SpriteAnimationClip& clip,
            int sortingOrder = 140,
            const std::function<void()>& onFinished = nullptr
        );

        void Start() override;
        void Play();
    private:
        SpriteAnimationClip clip;
        std::shared_ptr<SpriteAnimationComponent> animation;
        int sortingOrder;
        std::function<void()> onFinished;//10.16일 추가 애니메이션 이후에 재생 될 함수가 필요해서 추가
        
    };
}