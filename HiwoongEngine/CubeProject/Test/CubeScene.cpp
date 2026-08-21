#include "CubeScene.h"
#include "Cube/CubeObject.h"
namespace Hiwoong
{
	void CubeScene::SceneInitialize()
	{
		SetScreenSize(Vector2(40, 20));

		Instantiate<CubeObject>();

		Scene::SceneInitialize();
	}
}
