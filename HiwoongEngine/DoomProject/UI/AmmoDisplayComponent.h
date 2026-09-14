#pragma once

#include "Component/Component.h"
#include <memory>

namespace Hiwoong
{
	//포인터만 가져올꺼라서 전방선언
	class Player;

	class AmmoDisplayComponent : public Component
	{
		TYPE_DECALRATIONS(AmmoDisplayComponent, Component)

	public:
		explicit AmmoDisplayComponent(const std::shared_ptr<Player>& player);
		void Start() override;
	private:
		std::weak_ptr<Player> player;

	};
}
