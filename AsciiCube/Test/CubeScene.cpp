#include "CubeScene.h"
#include "Cube/CubeObject.h"
#include "Engine/Engine.h"
namespace Hiwoong
{
	void CubeScene::SceneInitialize()
	{
		// CubeScene
		SetScreenSize(Vector2(
			Engine::Get().GetWidth(),
			Engine::Get().GetHeight()
		));

		Instantiate<CubeObject>(
			Vector3(0, 0.5f, 5),
			Vector3(0, 0, 0),
			Vector3(1, 1, 1)
		);

		Scene::SceneInitialize();
	}
}
