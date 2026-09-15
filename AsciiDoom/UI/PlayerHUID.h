#pragma once

#include "GameObject/GameObject.h"
namespace Hiwoong
{

	class Player;
	class HPDisplay;
	class AmmoDisplay;

	//플레이어의 상태를 담는 UI
	//이 아래 HP표시 게임오브젝트와, AMMO표시 오브젝트를 각각 자식으로 둘꺼임.
	class PlayerHUID : public GameObject
	{
		TYPE_DECALRATIONS(PlayerHUID,GameObject)
		

	public:
		explicit PlayerHUID(const std::shared_ptr<Player>& player, int scale);

		void Start() override;

	private:
		std::weak_ptr<Player> player;
		std::weak_ptr<HPDisplay> hpDisplayObject;
		std::weak_ptr<AmmoDisplay> ammoDisplayObject;

		int scale; //문자 크기.
	};
}
