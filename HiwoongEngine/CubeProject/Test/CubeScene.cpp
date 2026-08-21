#include "CubeScene.h"
#include "Cube/CubeObject.h"
namespace Hiwoong
{
	void CubeScene::SceneInitialize()
	{
		// CubeScene
		SetScreenSize(Vector2(180, 110));

		Instantiate<CubeObject>();

		Scene::SceneInitialize();
	}
}
