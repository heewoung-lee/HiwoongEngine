#pragma once

#include "GameObject/GameObject.h"

namespace Hiwoong
{
	class Player;

	//플레이어의 HP를 나타낼 게임오브젝트 이걸로
	//뷰와 로직을 컴포넌트로 붙여서 HP에 대한 관리를 책임지게 할것임.
	class HPDisplay : public GameObject
	{
		TYPE_DECALRATIONS(HPDisplay, GameObject)


	public:
		explicit HPDisplay(const std::shared_ptr<Player>& player,int scale);


	};

}

