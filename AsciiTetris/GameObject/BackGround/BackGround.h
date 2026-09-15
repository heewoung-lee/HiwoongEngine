#pragma once
#include "GameObject/GameObject.h"
#include "Math/Vector2.h"

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

