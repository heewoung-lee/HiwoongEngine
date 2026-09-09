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
		player->SetMap(doomMap);
		//플레이어 포지션 셋팅.
		//플레이어는 둠 맵을 월드 포지션으로 두고, 둠맵이 조회한 플레이어 포지션으로 설정 
		player->SetPosition(
			doomMap->GetPlayerSpawnPosition()
		);

		std::shared_ptr<TransformComponent> playerTr = player->GetComponent<TransformComponent>();
		camera = std::make_unique<Camera3D>(*playerTr);
	}


	void DoomScene::SceneInitialize()
	{
		Scene::SceneInitialize();

		doomMap = Instantiate<DoomMap>("Assets/Maps/Level01.txt");

		//콜백 등록
		doomMap->AddOnMapBuilt([this]()
		{
			this->SpawnPlayer();
		});
	}

	void DoomScene::Update(double deltatime)
	{
		Scene::Update(deltatime);

		if (camera == nullptr) return;

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

		RenderView renderView(
			view,
			projection,
			screenSize,
			nearPlane
		);

		//기본 밝기 조정.
		renderView.ambientBrightness = 0.2f;
		renderView.directionalIntensity = 0.0f;

		renderView.spotLight.position = player->GetWorldPosition();
		renderView.spotLight.direction = camera->GetForward().Normalized();
		
		renderView.cameraToWorld =
			Matrix4x4::Translation(player->GetWorldPosition()) *
			Matrix4x4::RotationY(camera->GetYaw());

		meshRenderer.Render(
			doomMap->GetMapMesh(),
			Matrix4x4::Identity(),
			renderView
		);

	}

}

