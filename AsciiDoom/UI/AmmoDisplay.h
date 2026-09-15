#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
	class Player;

	class AmmoDisplay : public GameObject
	{
		TYPE_DECALRATIONS(AmmoDisplay,GameObject)

	public:
		explicit AmmoDisplay(const std::shared_ptr<Player>& player,int scale);
	};
}

