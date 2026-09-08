#pragma once
#include "Core/Core.h"
#include "GameObject/GameObject.h"
#include <memory>

namespace Hiwoong
{
	class DoomMap;

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

		void SetMap(const std::shared_ptr<DoomMap>& map)
		{
			doomMap = map;
		}
	private:
		std::shared_ptr<TransformComponent> transform;
		std::weak_ptr<DoomMap> doomMap;//충돌검사만 함므로 약참조로 벽 조회
		double frameDeltaTime;
		Vector3 direction;
		float collisionHalfSize = 0.2f;//충돌크기
	};
}

