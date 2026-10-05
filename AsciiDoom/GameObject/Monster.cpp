#include "Monster.h"
#include "Component/SpriteRenderer3DComponent.h"
#include "Component/SpriteAnimationComponent.h"
#include "Component/BoxCollider3DComponent.h"
#include "Component/NavAIComponent.h"
#include "Navigation/INavAIContext.h"
#include <memory>
#include "Scene/Scene.h"
#include <algorithm>
#include <cassert>

namespace Hiwoong
{
	Monster::Monster(
		const Vector3& spawnPosition, 
		const MonsterAnimationSet& animations,
		int hp, 
		float speed, 
		float attackRange,
		float scale) : GameObject(spawnPosition), animations(animations),currentHp(hp),speed(speed),attackRange(attackRange),scale(scale)
	{
		
		
	}

	void Monster::Start()
	{
		super::Start();

		transform->SetScale(
			Vector3(scale, scale, scale)
		);

		
		Vector3 size = InitSetColliderSize();
		AddComponent<BoxCollider3DComponent>(
			Vector3(
				size.x * 0.5f,
				size.y * 0.5f,
				size.z * 0.5f
			)
		);
		InitReferences();
	
	}
	void Monster::Update(double deltaTime)
	{
		super::Update(deltaTime);
		UpdateState();
	}
	void Monster::UpdateState()
	{
		const auto targetObject = target.lock();

		if (targetObject ==nullptr || targetObject->IsActive() == false)
		{
			ChangeState(MonsterState::Idle);
			return;
		}

		if (IsWithinAttackRange(targetObject->GetWorldPosition()))
		{
			ChangeState(MonsterState::Attack);
			return;
		}

		ChangeState(MonsterState::Run);
	}
	void Monster::InitReferences()
	{
		spriteRenderer = AddComponent<SpriteRenderer3DComponent>(
			animations.idle.frames.front()
		);
		navAIComponent = AddComponent<NavAIComponent>();
		animationComponent = AddComponent<SpriteAnimationComponent>(
			std::vector<AnimationBinding>{
				{ "Idle", animations.idle },
				{ "Run", animations.run },
				{ "Attack", animations.attack },
				{ "Dead", animations.dead }
		},
			"Idle"
		);

		const auto context =
			std::dynamic_pointer_cast<INavAIContext>(GetOwner());

		assert(context != nullptr);
		if (context == nullptr) return;

		target = context->GetNavigationTarget();

		assert(!target.expired());
	}

	bool Monster::ChangeState(MonsterState nextState)
	{
		if (currentState == nextState)
			return true;

		std::string animationName;

		switch (nextState)
		{
		case MonsterState::Idle:
			animationName = "Idle";
			break;

		case MonsterState::Run:
			animationName = "Run";
			break;

		case MonsterState::Attack:
			animationName = "Attack";
			break;

		case MonsterState::Dead:
			animationName = "Dead";
			break;
		}

		if (animationComponent->Play(animationName) == false)
			return false;

		currentState = nextState;
		return true;
	}

	bool Monster::IsWithinAttackRange(
		const Vector3& targetPosition
	) const
	{
		Vector3 difference = targetPosition - GetWorldPosition();
		difference.y = 0.0f;

		return difference.Length() <= attackRange;
	}

	

	void Monster::TakeDamage(int damage)
	{
		currentHp -= damage;

		if (currentHp <= 0)
		{
			//TODO: 죽음 애니메이션 실행.
			Destroy();
		}

	}

	Vector3 Monster::InitSetColliderSize()
	{
		assert(animations.idle.frames.empty() == false);

		const SpriteFrame& idleFrame = animations.idle.frames.front();

		assert(idleFrame.cells.empty() == false);

		std::size_t imageWidth = 0;

		for (const std::vector<SpriteCell>& row : idleFrame.cells)
		{
			imageWidth = (std::max)(imageWidth, row.size());
		}

		const float imageHeight = static_cast<float>(idleFrame.cells.size());

		const float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

		const float worldHeight = scale;
		const float worldWidth = scale * aspectRatio;


		return Vector3(worldWidth, worldHeight, worldWidth);
	}


}

