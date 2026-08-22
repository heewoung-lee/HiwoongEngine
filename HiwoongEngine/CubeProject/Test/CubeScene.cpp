#include "CubeScene.h"
#include "Cube/CubeObject.h"
namespace Hiwoong
{
	void CubeScene::SceneInitialize()
	{
		// CubeScene
		SetScreenSize(Vector2(180, 110));

		Instantiate<CubeObject>(
			Vector3(0, 0, 5),
			Vector3(0.4f, 0.6f, 0),
			Vector3(1, 1, 1)
		);

		Scene::SceneInitialize();
	}
}
