#include "EffectViewObject.h"
#include "Component/SpriteRenderer3DComponent.h"
#include "Component/SpriteAnimationComponent.h"
namespace Hiwoong
{
    EffectViewObject::EffectViewObject(
        const Vector3& position,
        const SpriteAnimationClip& animationClip
    )
        : GameObject(position),
        animationClip(animationClip)
    {
    }

    void EffectViewObject::Start()
    {
        super::Start();

        transform->SetScale(Vector3(0.35f, 0.35f, 0.35f));

        AddComponent<SpriteRenderer3DComponent>(
            animationClip.frames.front(),
            Color::Red
        );

        animationComponent = AddComponent<SpriteAnimationComponent>(
            animationClip.frames,
            animationClip.duration
        );

        animationComponent->AddOnAnimationEnd(
            [this]()
            {
                Destroy();
            }
        );

        
    }

    //애니메이션 실행을 start에서 하면 AddComponent한 컴포넌트들이 아직 초기화가
    // 안되어서 assert에 걸림. 그래서 플래그 변수를 통해. update로 애니메이션 재생처리함
    void EffectViewObject::Update(double deltaTime)
    {
        super::Update(deltaTime);

        if (animationComponent == nullptr ||
            animationComponent->HasStared() == false)
        {
            return;
        }

        if (hasPlayedAnimation == false)
        {
            hasPlayedAnimation = animationComponent->Play(animationClip);
        }


    }
}