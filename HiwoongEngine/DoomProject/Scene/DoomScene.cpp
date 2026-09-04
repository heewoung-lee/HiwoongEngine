#include "DoomScene.h"
#include "Scene/Scene.h"
#include "Math/MathConstants.h"
#include "GameObject/GameObject.h"
#include "Player/Player.h"

namespace Hiwoong
{
	DoomScene::DoomScene()
		: transform(),
		camera(transform),

		wallModel(Matrix4x4::Translation(Vector3(0, 0, 5))),

		rightWallModel(Matrix4x4::Translation(Vector3(2, 0, 3))*
			Matrix4x4::RotationY(MathConstants::Pi / 2.0f)),

		leftWallModel(Matrix4x4::Translation(Vector3(-2, 0, 3))*
			Matrix4x4::RotationY(3 * MathConstants::Pi / 2.0f)),

		floorModel(
			Matrix4x4::Translation(Vector3(0, 1, 3))*
			Matrix4x4::RotationX(3 * MathConstants::Pi / 2.0f)*
			Matrix4x4::Scale(Vector3(1.0f,2.0f,1.0f))
		),

		ceilingModel(
			Matrix4x4::Translation(Vector3(0, -1, 3))*
			Matrix4x4::RotationX(MathConstants::Pi / 2.0f)*
			Matrix4x4::Scale(Vector3(1.0f, 2.0f, 1.0f))
		)
	{
	}

	void DoomScene::SpawnPlayer()
	{
		player = Instantiate<Player>();
	}


	void DoomScene::SceneInitialize()
	{
		Scene::SceneInitialize();

		wallMesh.vertices =
		{
			Vertex{Vector3(-2,-1,0)},
			Vertex{Vector3(2,-1,0)},
			Vertex{Vector3(2,1,0)},
			Vertex{Vector3(-2,1,0)},
		};

		//법선 벡터가 -z축으로 나오는 방향으로 
		Triangle tri1 = { 0,2,1};
		Triangle tri2 = { 0,3,2 };

		wallMesh.triangles.emplace_back(tri1);
		wallMesh.triangles.emplace_back(tri2);


		SpawnPlayer();

	}

	void DoomScene::Update(double deltatime)
	{
		Scene::Update(deltatime);
		const Matrix4x4 view = camera.GetViewMatrix();

		const Vector2 screenSize = GetScreenSize();
		const float aspectRatio =static_cast<float>(screenSize.x) / screenSize.y;

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
			screenSize
		);

		meshRenderer.Render(wallMesh, wallModel, renderView);
		meshRenderer.Render(wallMesh, rightWallModel, renderView);
		meshRenderer.Render(wallMesh, leftWallModel, renderView);
		meshRenderer.Render(wallMesh, floorModel, renderView);
		meshRenderer.Render(wallMesh, ceilingModel, renderView);
	}

}

