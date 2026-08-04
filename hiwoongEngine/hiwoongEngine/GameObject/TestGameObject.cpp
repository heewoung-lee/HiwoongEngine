#include "TestGameObject.h"
#include <iostream>

void TestGameObject::Update(double deltaTime)
{
	GameObject::Update(deltaTime);

	std::cout << "FPS: " << (1 / deltaTime) << std::endl;
}
