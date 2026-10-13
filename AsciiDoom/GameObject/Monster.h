#pragma once

#include "GameObject/GameObject.h"
#include "Interfaces/IDamageable.h"
#include <vector>
#include <cstddef>

namespace Hiwoong
{
	class NavAIComponent;
	class ModelAnimationComponent;
	struct ModelData;

	class Monster : public GameObject, public IDamageable
	{
		TYPE_DECALRATIONS(Monster, GameObject)

	public:


		explicit Monster(
			const Vector3& spawnPosition,
			const std::shared_ptr<const ModelData>& model,
			int hp = 30,
			float speed = 1.0f,
			float attackRange = 1.0f,
			float scale = 0.7f
		);

		void Start() override;
		void Update(double deltaTime) override;
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
		std::shared_ptr<NavAIComponent> navAIComponent;
		std::weak_ptr<GameObject> target;//대부분은 플레이어


	private:
		void InitMeshRenderer();
		Vector3 InitSetColliderSize();
		void InitReferences();
		//유한상태머신 행동변경
		void ChangeState(MonsterState nextState);
		//공격이 가능한 거리인지 판별
		bool IsWithinAttackRange(
			const Vector3& targetPosition
		) const;
		void UpdateState();
		//경로요청 메서드
		void RequestPath();

		// 주어진 방향으로 몬스터의 몸을 돌린다.
		void UpdateFacing(const Vector3& direction, double deltaTime);

		void FollowPath(double deltaTime);

		std::vector<Vector3> path;
		std::size_t currentPathIndex = 0;

		//10.9일 몬스터에 대한 모델 fbx 추가
		std::shared_ptr<const ModelData> model;

		std::shared_ptr<ModelAnimationComponent> modelAnimationComponent;
	};



}
