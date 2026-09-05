#pragma once
#include "Core/Core.h"
#include "GameObject/GameObject.h"
#include <memory>

namespace Hiwoong
{
	class Player : public GameObject
	{
		TYPE_DECALRATIONS(Player,GameObject)

	public:
		Player();
		~Player() = default;

		void Start() override;
		void Update(double deltaTime) override;


		void TurnLeft(Vector3& rotation);
		void TurnRight(Vector3& rotation);

		void MoveForward(const Vector3& rotation);
		void MoveBack(const Vector3& rotation);
		void MoveLeft(const Vector3& rotation);
		void MoveRight(const Vector3& rotation);


	private:
		std::shared_ptr<TransformComponent> transform;
		double frameDeltaTime;
		Vector3 direction;
	};
}

