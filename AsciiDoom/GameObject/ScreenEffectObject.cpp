#include "ScreenEffectObject.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/SpriteAnimationComponent.h"
#include <cassert>
namespace Hiwoong
{
    ScreenEffectObject::ScreenEffectObject(
        const SpriteAnimationClip& clip,
        int sortingOrder,
        const std::function<void()>& onFinished
    )
        : clip(clip),
        sortingOrder(sortingOrder),
        onFinished(onFinished)
    {
    }
    void ScreenEffectObject::Start()
    {
        super::Start();

        assert(!clip.frames.empty());

        AddComponent<SpriteRendererComponent>(
            SpriteFrame({}, {}),
            sortingOrder
        );

        animation = AddComponent<SpriteAnimationComponent>(
            clip.frames,
            clip.duration
        );

        animation->AddOnAnimationEnd(onFinished);
    }
    void ScreenEffectObject::Play()
    {
        assert(animation != nullptr);

        animation->Play(clip);
    }
}

