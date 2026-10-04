#pragma once

#include "GameObject/GameObject.h"
#include "Interfaces/IDamageable.h"
#include "Animation/MonsterAnimationSet.h"

namespace Hiwoong
{
	class SpriteRenderer3DComponent;
	class SpriteAnimationComponent;
	class NavAIComponent;

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
			float scale = 0.7f //9.26일 잠깐 수정
		);

		void Start() override;
		void TakeDamage(int damage) override;

	private :
		enum class MonsterState
		{
			Idle,
			Run,
			Attack,
			Dead
		};

		MonsterState currentState = MonsterState::Idle;

		int currentHp;
		float speed;
		float attackRange;
		const float scale;
		MonsterAnimationSet animations;
		std::shared_ptr<SpriteRenderer3DComponent> spriteRenderer;
		std::shared_ptr<SpriteAnimationComponent> animationComponent;
		std::shared_ptr<NavAIComponent> navAIComponent;
		std::weak_ptr<GameObject> target;//대부분은 플레이어


	private:
		Vector3 InitSetColliderSize();
		void InitReferences();
		//유한상태머신
		bool ChangeState(MonsterState nextState);
		//공격이 가능한 거리인지 판별
		bool IsWithinAttackRange(
			const Vector3& targetPosition
		) const;
	};



}
