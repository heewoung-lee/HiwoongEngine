#include "TestScene.h"
#include "GameObject/TestGameObject.h";

void TestScene::SceneInitialize()
{
	Scene::SceneInitialize();

	Instantiate<TestGameObject>();
}
