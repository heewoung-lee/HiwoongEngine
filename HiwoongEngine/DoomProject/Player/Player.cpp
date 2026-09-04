#include "Player.h"
#include "Component/Transform3DComponent.h"

#include <iostream>


namespace Hiwoong
{
	Player::Player()
	{
	}
	void Player::Start()
	{
		super::Start();

		bool checkGetComponent = GetComponent<Transform3DComponent>() != nullptr;
		bool checkGetTr = GetTransform() == nullptr;


		std::cout << (checkGetComponent && checkGetTr) << std::endl;
	}

}

