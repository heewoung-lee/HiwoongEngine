#include "TestGameObject.h"
#include "GameObject/Player/PlayerInputComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Math/Vector3.h"

#include <iostream>

using namespace Hiwoong;

TestGameObject::TestGameObject(): GameObject(Vector3(5,5,0))
{
}

void TestGameObject::Start()
{
	super::Start();
	AddComponent<PlayerInputComponent>();
}
