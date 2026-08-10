#include "TestGameObject.h"
#include "GameObject/Player/PlayerInputComponent.h"
#include "Component/SpriteRendererComponent.h"

#include <iostream>

using namespace Hiwoong;

TestGameObject::TestGameObject(): GameObject(Vector2(5,5))
{
}

void TestGameObject::Start()
{
	super::Start();
	AddComponent<PlayerInputComponent>();
	AddComponent<SpriteRendererComponent>("P", Color::Green,5);
}