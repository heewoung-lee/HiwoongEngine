#pragma once
#include "Core/Core.h"
#include "GameObject/GameObject.h"


namespace Hiwoong
{
	class Player : public GameObject
	{
		TYPE_DECALRATIONS(Player,GameObject)

	public:
		Player();
		~Player() = default;

		void Start() override;

	};
}

