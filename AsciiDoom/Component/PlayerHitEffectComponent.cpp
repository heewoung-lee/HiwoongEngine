#include "PlayerHitEffectComponent.h"
#include "Player/Player.h"
#include "GameObject/ScreenEffectObject.h"
#include "Scene/Scene.h"
#include <cassert>
namespace Hiwoong
{
    PlayerHitEffectComponent::PlayerHitEffectComponent(
        const SpriteAnimationClip& clip
    )
        : clip(clip)
    {
    }

    void PlayerHitEffectComponent::Start()
    {
        super::Start();

        const auto player =
            std::dynamic_pointer_cast<Player>(GetOwner());

        assert(player != nullptr);

        previousHp = player->GetCurrentHp();

        screenEffect = player->Instantiate<ScreenEffectObject>(clip);

        std::weak_ptr<PlayerHitEffectComponent> weakEffect =
            GetComponent<PlayerHitEffectComponent>();

        //구독처리
        player->AddHPChangedCallback(
            [weakEffect](int hp)
            {
                const auto effect = weakEffect.lock();

                if (effect == nullptr)
                    return;

                effect->OnHpChanged(hp);
            }
        );
    }
    void PlayerHitEffectComponent::OnHpChanged(int hp)
    {
        //hp가 감소 했으니, 효과 발동
        if (hp < previousHp)
        {
            // 여기에 피격 애니메이션과 쉐이킹 요청을 연결할 예정.
            const auto effect = screenEffect.lock();

            if (effect != nullptr)
                effect->Play();
        }

        previousHp = hp;
    }
}