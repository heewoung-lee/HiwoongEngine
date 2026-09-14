#include "AmmoDisplay.h"
#include "Component/SpriteRendererComponent.h"
#include "BigTextDisplayComponent.h"
#include "AmmoDisplayComponent.h"
#include "Player/Player.h"
#include <string>
namespace Hiwoong
{
	AmmoDisplay::AmmoDisplay(const std::shared_ptr<Player>& player, int scale)
	{
		AddComponent<SpriteRendererComponent>("", Color::Green, 150);
		AddComponent<BigTextDisplayComponent>(
			"AMMO " + std::to_string(player->GetAmmo()), scale);
		AddComponent<AmmoDisplayComponent>(player);
	}

}
