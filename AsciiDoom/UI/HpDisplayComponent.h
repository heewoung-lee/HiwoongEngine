#pragma once

#include "Component/Component.h"
#include <memory>

namespace Hiwoong
{
	class Player;

	class HpDisplayComponent : public Component
	{
		TYPE_DECALRATIONS(HpDisplayComponent,Component)

			//주인인 플레이어를 통해 생성자 호출
		public:
			explicit HpDisplayComponent(
				const std::shared_ptr<Player>& player
			);

			void Start() override;
		
	private:
		std::weak_ptr<Player> player;

	};
}

