#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
	class Wall : public GameObject
	{
		TYPE_DECALRATIONS(Wall, GameObject)


	public:
		Wall(const Vector2& position);
		virtual ~Wall() = default;
	};
}
