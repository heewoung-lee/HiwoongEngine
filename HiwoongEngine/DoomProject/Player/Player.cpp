#include "Player.h"
#include "Component/TransformComponent.h"
#include <iostream>


namespace Hiwoong
{
	Player::Player()
	{
	}
	void Player::Start()
	{
		super::Start();

		bool checkGetComponent = GetComponent<TransformComponent>() != nullptr;
		bool checkGetTr = GetComponent<TransformComponent>() == nullptr;


		std::cout << (checkGetComponent && checkGetTr) << std::endl;
	}

}

