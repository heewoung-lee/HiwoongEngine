#include "ScreenEffectObject.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/SpriteAnimationComponent.h"
#include <cassert>
namespace Hiwoong
{

	ScreenEffectObject::ScreenEffectObject(const SpriteAnimationClip& clip)
		: clip(clip)
	{
	}
    void ScreenEffectObject::Start()
    {
        super::Start();

        assert(!clip.frames.empty());

        AddComponent<SpriteRendererComponent>(
            SpriteFrame({}, {}),
            140
        );

        animation = AddComponent<SpriteAnimationComponent>(
            clip.frames,
            clip.duration
        );
    }
    void ScreenEffectObject::Play()
    {
        assert(animation != nullptr);

        animation->Play(clip);
    }
}

