#pragma once
#include "Core/Core.h"
#include "GameObject/GameObject.h"
#include "Math/Vector2.h"
#include <memory>
#include <functional>
#include <vector>

namespace Hiwoong
{
	class DoomMap;

	class Player : public GameObject
	{
		TYPE_DECALRATIONS(Player,GameObject)

	public:
		explicit Player();
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

		void SetHp(int value);
		void SetAmmo(int value);

		inline int GetHp() const { return hp; }
		inline int GetAmmo() const { return ammo; }

		using OnHpChange = std::function<void(int)>;
		using OnAmmoChange = std::function<void(int)>;

		void AddHPChangedCallback(const OnHpChange& callback)
		{
			hpChangeCallbacks.push_back(callback);
		}

		void AddAmmoChangeCallback(const OnAmmoChange& callback)
		{
			ammoChangeCallbacks.push_back(callback);
		}


	private:
		std::shared_ptr<TransformComponent> transform;
		std::weak_ptr<DoomMap> doomMap;//충돌검사만 함므로 약참조로 벽 조회
		double frameDeltaTime;
		Vector3 direction;
		float collisionHalfSize = 0.2f;//충돌크기
		
		int hp = 100;
		int ammo = 24;

		//콜백
		std::vector<OnHpChange> hpChangeCallbacks;
		std::vector<OnAmmoChange> ammoChangeCallbacks;

	};
}

