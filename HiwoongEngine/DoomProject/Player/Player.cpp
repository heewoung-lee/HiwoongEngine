#include "Player.h"
#include "Component/TransformComponent.h"
#include "Core/Input.h"
#include "Math/MathConstants.h"
#include "Map/DoomMap.h"
#include "Component/BoxCollider3DComponent.h"
#include "Scene/Scene.h"
#include <iostream>
#include <cmath>

namespace Hiwoong
{
	Player::Player() : frameDeltaTime(0.0)
	{
	}
	void Player::Start()
	{
		super::Start();
		transform = GetComponent<TransformComponent>();
		AddComponent<BoxCollider3DComponent>(Vector3(collisionHalfSize, collisionHalfSize, collisionHalfSize));
	}

	void Player::Update(double deltaTime)
	{
		super::Update(deltaTime);
		frameDeltaTime = deltaTime;
		direction = Vector3(0, 0, 0);

		Vector3 rotation = transform->GetRotation();

		TurnLeft(rotation);
		TurnRight(rotation);
		transform->SetRotation(rotation);

		MoveForward(rotation);
		MoveBack(rotation);
		MoveLeft(rotation);
		MoveRight(rotation);

		if (direction.Length() > Epsilon)
		{
			direction = direction.Normalized();
		}

		const std::shared_ptr<Scene> scene = GetOwner();

		if (scene == nullptr)
			return;

		Vector3 movement = (direction * 3.0f * static_cast<float>(frameDeltaTime));
		Vector3 position = transform->GetWorldPosition();

		//X 방향 이동검사 
		Vector3 nextPosition = position;
		nextPosition.x += movement.x;

		if (scene->CanMoveTo(*this, nextPosition))
		{
			position = nextPosition;
		}

		//X 방향 이동 확인후에 Z방향 이동확인.
		nextPosition = position;
		nextPosition.z += movement.z;
		
		if (scene->CanMoveTo(*this, nextPosition))
		{
			position = nextPosition;
		}

		//X방향 Z방향 각각확인후에 포지션 적용. //벽을 따라 미끄러짐
		transform->SetWorldPosition(position);
	}

	void Player::TurnLeft(Vector3& rotation)
	{
		if (Input::Get().GetKey('Q'))
		{
			rotation.y -= MathConstants::Pi * 0.5f
				* static_cast<float>(frameDeltaTime);
		}
	}

	void Player::TurnRight(Vector3& rotation)
	{
		if (Input::Get().GetKey('E'))
		{
			rotation.y += MathConstants::Pi * 0.5f
				* static_cast<float>(frameDeltaTime);
		}
	}

	void Player::MoveForward(const Vector3& rotation)
	{
		if (Input::Get().GetKey('W'))
		{
			//yaw == 0일때 (0,0,1) 기준의 전방은 x = sin , z = cos
			Vector3 forward(
				std::sin(rotation.y),
				0,
				std::cos(rotation.y)
			);
			direction += forward;
		}
	}

	void Player::MoveBack(const Vector3& rotation)
	{
		if (Input::Get().GetKey('S'))
		{
			Vector3 back(
				-std::sin(rotation.y),
				0,
				-std::cos(rotation.y)
			);
			direction += back;
		}

	}

	void Player::MoveRight(const Vector3& rotation)
	{
		if (Input::Get().GetKey('D'))
		{
			//yaw == 0일때 (1,0,0) 기준의 오른쪽은 x = sin , z = cos
			Vector3 right(
				std::cos(rotation.y),
				0,
				-std::sin(rotation.y)
			);
			direction += right;
		}
		
	}


	void Player::MoveLeft(const Vector3& rotation)
	{
		if (Input::Get().GetKey('A'))
		{
			Vector3 left(
				-std::cos(rotation.y),
				0,
				std::sin(rotation.y)
			);
			direction += left;
		}
	}
}

