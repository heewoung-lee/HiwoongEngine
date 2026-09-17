#include "AmmoDisplayComponent.h"
#include "Player/Player.h"
#include "BigTextDisplayComponent.h"
#include <cassert>
#include <string>

namespace Hiwoong
{
	AmmoDisplayComponent::AmmoDisplayComponent(const std::shared_ptr<Player>& player) :player(player)
	{

	}

	void AmmoDisplayComponent::Start()
	{
		super::Start();
		const std::shared_ptr<Player> targetPlayer = player.lock();
		assert(targetPlayer != nullptr);

		const std::shared_ptr<BigTextDisplayComponent> display = GetComponent<BigTextDisplayComponent>();
		assert(display != nullptr);

		display->SetText("AMMO " + std::to_string(targetPlayer->GetCurrentAmmo()));


		std::weak_ptr<BigTextDisplayComponent> weakDisplay = display;

		//변화량이 있으면 콜백받도록 등록
		targetPlayer->AddAmmoChangeCallback(
			[weakDisplay](int ammo)
			{
				const std::shared_ptr<BigTextDisplayComponent> display = weakDisplay.lock();
				if (display == nullptr) return;

				display->SetText("AMMO " + std::to_string(ammo));
			}
		);
	}

}
