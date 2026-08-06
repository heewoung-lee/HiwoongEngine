#include "TestGameObject.h"
#include "Core/Input.h"
#include <iostream>
#include <windows.h>

using namespace Hiwoong;

TestGameObject::TestGameObject(): GameObject("P",Vector2(5,5),Color::Green)
{
	sortingOrder = 5;
}

void TestGameObject::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}

	if (Input::Get().GetKey(VK_LEFT) && position.x > 0)
	{
		position.x -= 1;
	}

	if (Input::Get().GetKey(VK_RIGHT) && position.x < 39)
	{
		position.x += 1;
	}

	if (Input::Get().GetKey(VK_UP) && position.y > 0 )
	{
		position.y -= 1;
	}


	if (Input::Get().GetKey(VK_DOWN) && position.y < 24)
	{
		position.y += 1;
	}
}
