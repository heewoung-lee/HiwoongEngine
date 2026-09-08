#pragma once

#include "Scene/Scene.h"
#include "Camera/Camera3D.h"
#include "Render/MeshRenderer.h"
#include "Player/Player.h"
#include "Map/DoomMap.h"
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
		

	private:
		void SpawnPlayer();

	private :
		std::shared_ptr<Player> player;
		std::unique_ptr<Camera3D> camera;

		MeshRenderer meshRenderer;

		std::shared_ptr<DoomMap> doomMap;

	};


}
