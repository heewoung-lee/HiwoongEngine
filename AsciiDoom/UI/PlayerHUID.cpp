#include "PlayerHUID.h"
#include "UI/HPDisplay.h"
#include "UI/AmmoDisplay.h"
#include "Scene/Scene.h"
#include "UI/BigTextDisplayComponent.h"
#include "Scene/DoomScene.h"
#include <cassert>

namespace Hiwoong
{
	PlayerHUID::PlayerHUID(const std::shared_ptr<Player>& player, int scale) : player(player), scale(scale)
	{
		assert(player != nullptr);
		assert(scale >= 1);
	}


	void PlayerHUID::Start()
	{
		super::Start();

		const std::shared_ptr<Player> targetPlayer = player.lock();
		assert(targetPlayer != nullptr);

		const std::shared_ptr<HPDisplay> hpDisplay = Instantiate<HPDisplay>(targetPlayer, scale);
		assert(hpDisplay != nullptr);

		//부모설정.
		hpDisplay->SetParent(shared_from_this(), false);


		const std::shared_ptr<AmmoDisplay> ammoDisplay = Instantiate<AmmoDisplay>(targetPlayer, scale);
		assert(ammoDisplay != nullptr);


		ammoDisplay->SetParent(shared_from_this(), false);

		hpDisplayObject = hpDisplay;
		ammoDisplayObject = ammoDisplay;

		//C++ 그지가튼거 꺼내기 엄청 힘드네
		const std::shared_ptr<HPDisplay> hpObject = hpDisplayObject.lock();
		assert(hpObject != nullptr);

		const std::shared_ptr<BigTextDisplayComponent> hpText
			= hpObject->GetComponent<BigTextDisplayComponent>();

		const std::shared_ptr<AmmoDisplay> ammoObject = ammoDisplayObject.lock();
		assert(ammoObject != nullptr);

		const std::shared_ptr<BigTextDisplayComponent> ammoText
			= ammoObject->GetComponent<BigTextDisplayComponent>();

		assert(hpText != nullptr);
		assert(ammoText != nullptr);

		const Vector2 hpSize = hpText->GetTextSize();
		const Vector2 ammoSize = ammoText->GetTextSize();


		const std::shared_ptr<DoomScene> scene =
			std::dynamic_pointer_cast<DoomScene>(GetOwner());

		assert(scene != nullptr);
		
		int itemSpacing = 10; // 배율 적용 전 HP와 AMMO 사이 간격

		//전체 스크린 - 게임영역높이 = UI높이
		const int uiHeight = scene->GetScreenSize().y - scene->GetGameSize().y;

		const int gap = itemSpacing * scale;
		const int totalWidth = hpSize.x + gap + ammoSize.x;

		const int startX = (scene->GetScreenSize().x - totalWidth) / 2;

		hpObject->SetPosition(Vector3(
			startX,
			(uiHeight - hpSize.y) / 2,
			0
		));

		ammoObject->SetPosition(Vector3(
			startX + hpSize.x + gap,
			(uiHeight - ammoSize.y) / 2,
			0
		));

	}

}
