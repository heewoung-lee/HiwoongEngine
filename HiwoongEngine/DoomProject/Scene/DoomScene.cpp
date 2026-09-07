#include "DoomScene.h"
#include "Scene/Scene.h"
#include "Math/MathConstants.h"
#include "GameObject/GameObject.h"
#include "Player/Player.h"
#include "Engine/Engine.h"
namespace Hiwoong
{
	DoomScene::DoomScene()
	{
	}

	void DoomScene::SpawnPlayer()
	{
		player = Instantiate<Player>();

		std::shared_ptr<TransformComponent> playerTr = player->GetComponent<TransformComponent>();
		camera = std::make_unique<Camera3D>(*playerTr);
	}




	void DoomScene::SceneInitialize()
	{
		Scene::SceneInitialize();

		doomMap = Instantiate<DoomMap>("Assets/Maps/Level01.txt");

		SpawnPlayer();
	}

	void DoomScene::Update(double deltatime)
	{
		Scene::Update(deltatime);
		const Matrix4x4 view = camera->GetViewMatrix();

		const Vector2 screenSize = GetScreenSize();
		const float aspectRatio = Engine::Get().GetScreenAspectRatio();

		const float fieldOfView = 60.0f * MathConstants::Pi / 180.0f;

		const float nearPlane = 0.1f;
		const float farPlane = 100.0f;


		const Matrix4x4 projection = Matrix4x4::Perspective(
			fieldOfView,
			aspectRatio,
			nearPlane,
			farPlane
		);

		const RenderView renderView(
			view,
			projection,
			screenSize,
			nearPlane
		);

		meshRenderer.Render(
			doomMap->GetMapMesh(),
			Matrix4x4::Identity(),
			renderView
		);

	}

}

