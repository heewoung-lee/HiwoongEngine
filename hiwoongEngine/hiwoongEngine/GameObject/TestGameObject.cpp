#include "TestGameObject.h"
#include "Core/Input.h"
#include <iostream>
#include <windows.h>

using namespace Hiwoong;

void TestGameObject::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}


	if (Input::Get().GetKeyDown('A'))
	{
		std::cout << "'A' Å° ´­¸²" << std::endl;
	}


	if (Input::Get().GetKeyUP('A'))
	{
		std::cout << "'A' Å° ¶¼Áü" << std::endl;
	}

	if (Input::Get().GetKey('A'))
	{
		std::cout << "'A' Å°" << std::endl;
	}

}
