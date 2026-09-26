#pragma once

#include "GameObject/GameObject.h"
#include "Interfaces/IDamageable.h"
#include "Animation/MonsterAnimationSet.h"

namespace Hiwoong
{
	class SpriteRenderer3DComponent;
	class SpriteAnimationComponent;

	class Monster : public GameObject, public IDamageable
	{
		TYPE_DECALRATIONS(Monster, GameObject)

	public:
		explicit Monster(
			const Vector3& spawnPosition,
			const MonsterAnimationSet& animations,
			int hp = 30,
			float speed = 1.0f,
			float attackRange = 1.0f,
			float scale = 0.7f //9.26¿œ ¿·±Ò ºˆ¡§
		);

		void Start() override;
		void TakeDamage(int damage) override;

	private:
		Vector3 InitSetColliderSize();

	private :
		int currentHp;
		float speed;
		float attackRange;
		const float scale;
		MonsterAnimationSet animations;
		std::shared_ptr<SpriteRenderer3DComponent> spriteRenderer;
		std::shared_ptr<SpriteAnimationComponent> animationComponent;
	};

}
