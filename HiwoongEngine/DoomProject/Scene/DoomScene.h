#pragma once

#include "Scene/Scene.h"
#include "Component/Transform3DComponent.h"
#include "Camera/Camera3D.h"
#include "Render/MeshRenderer.h"


namespace Hiwoong
{
	class DoomScene : public Scene
	{
	public:
		DoomScene();
		~DoomScene() override = default;


		void SceneInitialize() override;
		void Update(double deltatime) override;
		

	private :
		Transform3DComponent transform;
		Camera3D camera;
		MeshRenderer meshRenderer;
		Mesh wallMesh;


		//테스트용
		Matrix4x4 wallModel;
		Matrix4x4 rightWallModel;
		Matrix4x4 leftWallModel;
		Matrix4x4 floorModel;
		Matrix4x4 ceilingModel;
	};


}
