#include "Monster.h"
#include "Component/BoxCollider3DComponent.h"
#include "Component/NavAIComponent.h"
#include "Navigation/INavAIContext.h"
#include "ModelImport/ModelData.h"
#include "ModelImport/ModelMeshConverter.h"
#include "Component/MeshRenderComponent.h"
#include "Scene/Scene.h"
#include <memory>
#include <cassert>

namespace Hiwoong
{
	Monster::Monster(
		const Vector3& spawnPosition,
		const std::shared_ptr<const ModelData>& model,
		int hp,
		float speed,
		float attackRange,
		float scale
	)
		: GameObject(spawnPosition),
		currentHp(hp),
		speed(speed),
		attackRange(attackRange),
		scale(scale),
		model(model)
	{
		assert(this->model != nullptr);
	}

	void Monster::Start()
	{
		super::Start();

		transform->SetScale(
			Vector3(scale, scale, scale)
		);
		InitMeshRenderer();
		
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
		navAIComponent = AddComponent<NavAIComponent>();

		const auto context =
			std::dynamic_pointer_cast<INavAIContext>(GetOwner());

		assert(context != nullptr);
		if (context == nullptr) return;

		target = context->GetNavigationTarget();

		assert(!target.expired());
	}

	void Monster::ChangeState(MonsterState nextState)
	{
		if (currentState == nextState)
			return;

		currentState = nextState;
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

	void Monster::InitMeshRenderer()
	{
		assert(!model->meshes.empty());

		for (const ModelMesh& modelMesh : model->meshes)
		{
			AddComponent<MeshRenderComponent>(
				ConvertToRenderMesh(modelMesh),
				Color::Red
			);
		}
	}
	Vector3 Monster::InitSetColliderSize()
	{
		return Vector3(
			0.68f * scale,
			1.8f * scale,
			0.68f * scale
		);
	}


}

