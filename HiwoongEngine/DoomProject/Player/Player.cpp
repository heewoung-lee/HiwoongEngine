#include "Player.h"
#include "Component/TransformComponent.h"
#include "Core/Input.h"
#include "Math/MathConstants.h"
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

		Vector3 movement = (direction * 3.0f * static_cast<float>(frameDeltaTime));

		Vector3 curPos = transform->GetLocalPosition();
		transform->SetLocalPosition(curPos + movement);//이동

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

