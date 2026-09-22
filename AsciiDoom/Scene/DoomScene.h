#pragma once

#include "Scene/Scene.h"
#include "Camera/Camera3D.h"
#include "Render/MeshRenderer.h"
#include "Player/Player.h"
#include "Map/DoomMap.h"
#include "Math/MathConstants.h"
#include <memory>

namespace Hiwoong
{
	class DoomScene : public Scene
	{
	public:
		DoomScene();
		~DoomScene() override = default;


		void SceneInitialize() override;
		void Update(double deltatime) override;
		inline Vector2 GetGameSize() const
		{
			return gameSize;
		}
		Camera3D* GetCamera() const
		{
			return camera.get();
		}
	private:
		void SpawnPlayer();
		//모든 오브젝트의 렌더링을 갱신하는 함수.
		void RenderMeshes(const RenderView& renderView);

		//테스트 스폰
		void SpawnTestMonster();

	private :
		std::shared_ptr<Player> player;
		std::unique_ptr<Camera3D> camera;

		MeshRenderer meshRenderer;

		std::shared_ptr<DoomMap> doomMap;

		Vector2 gameSize = Vector2::Zero;
		//TODO:플레이어 상태UI의 크기 우선 하드코딩;; 잘되면 바꿀것
		float hudHeight = 10.0f;
	};


}
