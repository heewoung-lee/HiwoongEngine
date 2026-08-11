#pragma once
#include "GameObject/GameObject.h"


namespace Hiwoong
{
	class BackGround : public GameObject
	{
		TYPE_DECALRATIONS(BackGround,GameObject)

	public:
		BackGround(const Vector2& position);
		virtual ~BackGround() = default;
	};
}

