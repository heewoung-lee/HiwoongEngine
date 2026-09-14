#include "HpDisplayComponent.h"
#include "Player/Player.h"
#include "GameObject/GameObject.h"
#include "Component/SpriteRendererComponent.h"
#include "BigTextDisplayComponent.h"
#include "Component/Component.h"
#include <string>
#include <cassert>
namespace Hiwoong
{

	HpDisplayComponent::HpDisplayComponent(const std::shared_ptr<Player>& player): player(player)
	{
	}

	void HpDisplayComponent::Start()
	{
		super::Start();

		const std::shared_ptr<Player> targetPlayer = player.lock();
		assert(targetPlayer != nullptr);

		const std::shared_ptr<BigTextDisplayComponent> display = GetComponent<BigTextDisplayComponent>();
		assert(display != nullptr);

		//현재 플레이어의 HP를 처음 표시.
		display->SetText("HP " + std::to_string(targetPlayer->GetHp()));

		std::weak_ptr<BigTextDisplayComponent> weakDisplay = display;

		//구독 등록 콜백 때문에 UI가 계속 살아 있지 않게 하기 위해 weak_ptr로 받음
		targetPlayer->AddHPChangedCallback(
			[weakDisplay](int hp)
			{
				const auto display = weakDisplay.lock();
				if (display == nullptr) return;
				display->SetText("HP " + std::to_string(hp));
			}
		);

	}

}

