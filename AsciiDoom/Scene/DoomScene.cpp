#include "DoomScene.h"
#include "Scene/Scene.h"
#include "Math/MathConstants.h"
#include "GameObject/GameObject.h"
#include "Player/Player.h"
#include "Engine/Engine.h"
#include "UI/PlayerHUID.h"
#include "Render/Renderer.h"
#include "Render/IRenderable3D.h"
#include "GameObject/Bullet.h"

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
		// 둠 프로젝트에서 사용할 시야각을 지정한다.
		const float fieldOfView =
			60.0f * MathConstants::Pi / 180.0f;

		camera = std::make_unique<Camera3D>(*playerTr, fieldOfView);

		//플레이어의 상태UI
		const int hudScale = 1;

		//PlayerHUID의 위치는 늘어난곳에 위치 해야함.
		const auto hud = Instantiate<PlayerHUID>(player, hudScale);

		hud->SetPosition(Vector3(
			0.0f,
			gameSize.y,
			0.0f
		));
	}

	//TODO: 렌더링 방식이 정리 되고 안정화 되면, 부모 씬으로 격상시킬것.
	void DoomScene::RenderMeshes(const RenderView& renderView)
	{
		//씬에 있는 오브젝트 리스트들을 순회해서. IRenderable3D를 찾고 렌더링한다.
		for (const std::shared_ptr<GameObject>& obj : gameObjectList)
		{
			if (obj == nullptr || obj->IsActive() == false) continue;

			for (const std::shared_ptr<Component>& component : obj->GetComponents())
			{
				//초기화 안된 컴포넌트는 조회하면 안됨.
				if (component == nullptr || component->HasStared() == false) continue;

				const std::shared_ptr<IRenderable3D> renderable =
					std::dynamic_pointer_cast<IRenderable3D>(component);

				if (renderable == nullptr) continue;

				meshRenderer.Render(*renderable, renderView);
			}
		}
	}

	void DoomScene::SceneInitialize()
	{
		Scene::SceneInitialize();

		gameSize = GetScreenSize();

		// 기존 게임 영역 아래에 UI 공간을 추가한다.
		SetScreenSize(Vector2(
			gameSize.x,
			gameSize.y + hudHeight
		));


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
	
		const Vector2 screenSize = gameSize;
		const Vector2 characterSize = Renderer::Get().GetCharacterSize();

		const float aspectRatio =
			(static_cast<float>(gameSize.x) / gameSize.y) *
			(static_cast<float>(characterSize.x) / characterSize.y);

		const float fieldOfView = camera->GetFieldOfView();

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

		//메쉬렌더러가 붙여진 컴포넌트들의 메쉬렌더링 업데이트 
		RenderMeshes(renderView);
	}

}

