#include "Monster.h"
#include "Component/BoxCollider3DComponent.h"
#include "Component/NavAIComponent.h"
#include "Navigation/INavAIContext.h"
#include "ModelImport/ModelData.h"
#include "ModelImport/ModelMeshConverter.h"
#include "Component/MeshRenderComponent.h"
#include "Component/ModelAnimationComponent.h"
#include "Math/MathUtils.h"
#include "Scene/Scene.h"
#include "Math/MathConstants.h"
#include "GameObject/AttackObject.h"
#include <cmath>
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
		float scale,
		int attackDamage
	)
		: GameObject(spawnPosition),
		model(model),
		currentHp(hp),
		speed(speed),
		attackRange(attackRange),
		scale(scale),
		attackDamage(attackDamage)
	{
		assert(this->model != nullptr);
	}

	void Monster::Start()
	{
		super::Start();

		transform->SetScale(
			Vector3(scale, scale, scale)
		);
		modelAnimationComponent =
			AddComponent<ModelAnimationComponent>(model, "Idle");

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

		//매개변수 몬스터 데미지
		attackObject = Instantiate<AttackObject>(attackDamage);
		attackObject->SetParent(shared_from_this(), false);

		//몬스터 공격범위의 콜라이더 위치 설정
		UpdateAttackPosition();
		//애니메이션 이벤트 삽입
		InitAttackEvents();
		InitDeathEvent();
	}
	void Monster::UpdateFacing(
		const Vector3& direction,
		double deltaTime)
	{
		const float targetAngle =
			std::atan2(direction.x, direction.z);

		const float currentAngle = transform->GetRotation().y;
		const float ratio = 5.0f * static_cast<float>(deltaTime);

		const float nextAngle = MathUtils::LerpAngle(
			currentAngle,
			targetAngle,
			ratio
		);

		transform->SetRotation(Vector3(0.0f, nextAngle, 0.0f));

		UpdateAttackPosition();

	}
	void Monster::Update(double deltaTime)
	{
		super::Update(deltaTime);

		if (currentState == MonsterState::Dead)
		{
			UpdateDeath(deltaTime);
			return;
		}


		UpdateState();

		if (currentState != MonsterState::Run)
		{
			path.clear();
			return;
		}

		// 경로가 없거나 끝까지 이동했다면 다시 요청한다.
		if (currentPathIndex >= path.size())
		{
			RequestPath();
		}

		FollowPath(deltaTime);
	}
	void Monster::UpdateState()
	{
		if (currentState == MonsterState::Dead)
			return;


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
		bool accepted = false;

		if (nextState == MonsterState::Dead)
		{
			attackObject->EndAttack();
			modelAnimationComponent->Stop();
			accepted = modelAnimationComponent->Play("Dead", true);
		}
		else if (nextState == MonsterState::Run)
		{
			accepted = modelAnimationComponent->Play("Move");
		}
		else if (nextState == MonsterState::Attack)
		{
			accepted = modelAnimationComponent->Play("Attack", true);
		}
		else
		{
			accepted = modelAnimationComponent->Play("Idle");
		}

		if (accepted)
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
		assert(damage >= 0);

		if (currentState == MonsterState::Dead)
			return;

		currentHp -= damage;

		if (currentHp <= 0)
		{
			ChangeState(MonsterState::Dead);
		}
	}
	void Monster::RequestPath()
	{
		const auto targetObject = target.lock();
		if (targetObject == nullptr)
			return;

		path = navAIComponent->FindPath(
			GetWorldPosition(),
			targetObject->GetWorldPosition()
		);

		// 이동 중 몬스터의 높이는 유지한다.
		for (Vector3& position : path)
		{
			position.y = GetWorldPosition().y;
		}

		currentPathIndex = 0;
	}
	void Monster::UpdateAttackPosition()
	{
		const Vector3 offset =
			transform->GetForward() * attackRange;

		attackObject->SetPosition(offset);
	}
	void Monster::FollowPath(double deltaTime)
	{
		if (currentPathIndex >= path.size())
			return;

		const Vector3 currentPosition = GetWorldPosition();

		Vector3 direction = path[currentPathIndex] - currentPosition;
		direction.y = 0.0f;

		const float distance = direction.Length();

		// 현재 경유지에 도착했다면 다음 경유지로 넘어간다.
		if (distance <= 0.01f)
		{
			++currentPathIndex;
			return;
		}

		float moveDistance = speed * static_cast<float>(deltaTime);

		// 경유지를 지나치지 않도록 이동 거리를 제한한다.
		if (moveDistance > distance)
			moveDistance = distance;

		const Vector3 nextPosition =
			currentPosition + direction.Normalized() * moveDistance;

		const auto scene = GetOwner();
		assert(scene != nullptr);
		if (scene == nullptr)
			return;

		// 이동이 막히면 기존 경로를 비운다.
		if (scene->CanMoveTo(*this, nextPosition) == false)
		{
			path.clear();
			return;
		}
		//회전 갱신
		UpdateFacing(direction, deltaTime);
		transform->SetWorldPosition(nextPosition);
	}

	void Monster::InitAttackEvents()
	{
		modelAnimationComponent->AddAnimationEvent(
			"Attack",
			0.45,
			[this]()
			{
				attackObject->BeginAttack();
			}
		);

		modelAnimationComponent->AddAnimationEvent(
			"Attack",
			0.5,
			[this]()
			{
				attackObject->EndAttack();
			}
		);
	}

	void Monster::InitMeshRenderer()
	{
		assert(!model->meshes.empty());

		for (std::size_t meshIndex = 0;
			meshIndex < model->meshes.size();
			++meshIndex)
		{
			const ModelMesh& modelMesh = model->meshes[meshIndex];

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

				// 만든 렌더러를 변수에 보관한다.
				const auto renderer = AddComponent<MeshRenderComponent>(
					mesh,
					Color::White,
					texture
				);

				// 애니메이션 컴포넌트에게 연결할 렌더러를 알려준다.
				modelAnimationComponent->BindRenderer(meshIndex, renderer);
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
	void Monster::InitDeathEvent()
	{
		modelAnimationComponent->AddAnimationEvent(
			"Dead",
			1.0,
			[this]()
			{
				deathAnimationFinished = true;
			}
		);
	}
	void Monster::UpdateDeath(double deltaTime)
	{
		if (deathAnimationFinished == false)
			return;

		deathElapsedTime += deltaTime;

		if (deathElapsedTime >= deathDestroyDelay)
		{
			Destroy();
		}
	}
}

