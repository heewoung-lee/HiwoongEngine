#include "AttackObject.h"
#include "Component/BoxCollider3DComponent.h"

namespace Hiwoong
{
    AttackObject::AttackObject(int damage)
        : damage(damage)
    {
        isActive = false;
    }

    void AttackObject::Start()
    {
        super::Start();

        AddComponent<BoxCollider3DComponent>(
            Vector3(0.4f, 0.4f, 0.4f),
            true
        );
    }

    void AttackObject::OnCollision(
        const std::shared_ptr<GameObject>& other
    )
    {
        if (!IsActive())
            return;

        super::OnCollision(other);

        // 부모로 연결할 공격자 자신은 제외한다.
        if (other == GetParent())
            return;

        const auto target =
            std::dynamic_pointer_cast<IDamageable>(other);

        if (target == nullptr)
            return;

        // 이번 공격에서 이미 맞은 대상은 제외한다.
        for (const auto& hitTarget : hitTargets)
        {
            if (hitTarget.lock() == other)
                return;
        }

        hitTargets.push_back(other);
        target->TakeDamage(damage);
    }

    void AttackObject::BeginAttack()
    {
        hitTargets.clear();
        isActive = true;
    }

    void AttackObject::EndAttack()
    {
        isActive = false;
    }

}

