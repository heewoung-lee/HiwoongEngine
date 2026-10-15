#include "PlayerHitEffectComponent.h"
#include "Player/Player.h"
#include "GameObject/ScreenEffectObject.h"
#include "Scene/Scene.h"
#include "Camera/Camera3D.h"
#include "Scene/DoomScene.h"
#include <cassert>

namespace Hiwoong
{
    PlayerHitEffectComponent::PlayerHitEffectComponent(
        const SpriteAnimationClip& clip,
        float shakeStrength,
        double shakeDuration
    )
        : clip(clip),
        shakeStrength(shakeStrength),
        shakeDuration(shakeDuration)
    {
    }
    void PlayerHitEffectComponent::Update(double deltaTime)
    {
        super::Update(deltaTime);

        //카메라 쉐이킹
        camera->Update(deltaTime);
    }
    void PlayerHitEffectComponent::Start()
    {
        super::Start();

        const auto scene =
            std::dynamic_pointer_cast<DoomScene>(GetScene());

        assert(scene != nullptr);

        camera = scene->GetCamera();

        assert(camera != nullptr);


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

            camera->Shake(shakeStrength, shakeDuration);
        }

        previousHp = hp;
    }
}