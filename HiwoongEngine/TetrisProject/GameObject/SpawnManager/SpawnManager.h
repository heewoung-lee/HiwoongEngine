#pragma once

#include "Math/Vector2.h"
#include "GameObject/GameObject.h"

namespace Hiwoong
{
	class SpawnManager : public GameObject
	{
		TYPE_DECALRATIONS(SpawnManager, GameObject)
	public:
		SpawnManager(const Vector2& position);
		~SpawnManager() override = default;
		


		inline Vector2 SpawnPosition()const { return position; }


		void Start() override;

	};



}

