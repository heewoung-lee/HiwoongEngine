#include "TestScene.h"
#include "GameObject/TestGameObject.h"
#include "Math/Vector2.h"

namespace Hiwoong
{
	TestScene::TestScene() : Scene(Vector2(60,30))
	{
	}
	TestScene::~TestScene()
	{
	}
	void TestScene::SceneInitialize()
	{
		Scene::SceneInitialize();

		Instantiate<TestGameObject>();
	}

}

