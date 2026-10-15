#pragma once

#include "GameObject/GameObject.h"
#include "Interfaces/IDamageable.h"

namespace Hiwoong
{
    class AttackObject : public GameObject
    {
        TYPE_DECALRATIONS(AttackObject, GameObject)

    public:
        explicit AttackObject(int damage);

        void Start() override;

        void OnCollision(
            const std::shared_ptr<GameObject>& other
        ) override;

        void BeginAttack();
        void EndAttack();

    private:
        int damage;
        //Å¸°Ù 
        std::vector<std::weak_ptr<GameObject>> hitTargets;
    };
}