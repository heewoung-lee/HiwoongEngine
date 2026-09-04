#pragma once

#include "GameObject/GameObject.h"
#include "Math/Vector2.h"
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
