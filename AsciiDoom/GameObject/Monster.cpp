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
			std::size_t materialCount = modelMesh.diffuseTextures.size();

			if (materialCount == 0)
				materialCount = 1;

			for (std::size_t i = 0; i < materialCount; ++i)
			{
				Mesh mesh = ConvertToRenderMesh(modelMesh, i);

				if (mesh.triangles.empty())
					continue;

				std::shared_ptr<const ModelTexture> texture;

				if (i < modelMesh.diffuseTextures.size())
					texture = modelMesh.diffuseTextures[i];

				AddComponent<MeshRenderComponent>(
					mesh,
					Color::White,
					texture
				);
			}
		}
	}
	//실제 모델 높이로 충돌체 크기 맞추기
	Vector3 Monster::InitSetColliderSize()
	{
		float halfHeight = 0.0f;

		for (const ModelMesh& mesh : model->meshes)
		{
			for (const ModelVertex& vertex : mesh.vertices)
			{
				const float distance = std::abs(vertex.y);

				if (distance > halfHeight)
				{
					halfHeight = distance;
				}
			}
		}

		assert(halfHeight > 0.0f);

		return Vector3(
			0.68f * scale,
			halfHeight * 2.0f * scale,
			0.68f * scale
		);
	}


}

