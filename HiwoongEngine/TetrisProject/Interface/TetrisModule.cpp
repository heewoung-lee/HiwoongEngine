#include "TetrisModule.h"
#include "GameObject/TetrisBlocks/Block.h"
#include "Scene/Scene.h"
#include "GameObject/Player/PlayerInputComponent.h"
#include <cassert>
namespace Hiwoong
{
	void TetrisModule::Start()
	{
		GameObject::Start();
		AddComponent<PlayerInputComponent>();

		Color currentColor = GetColor();
		std::shared_ptr<Scene> currentScene = GetOnwer();

		assert(currentScene != nullptr);
		if (currentScene == nullptr)
		{
			return;
		}


		for (const Vector2& position : GetBlockPosition())
		{
			auto block = currentScene->Instantiate<Block>(
				position,
				currentColor
			);

			block->AttachTo(shared_from_this(), false);
		}
	}
	void TetrisModule::Update(double deltaTime)
	{
		GameObject::Update(deltaTime);

		elapsedDropTime += deltaTime;

		if (elapsedDropTime < dropInterval)
		{
			return;
		}

		elapsedDropTime -= dropInterval;

		Vector2 newPosition = GetPosition();
		newPosition.y += 1;

		SetPosition(newPosition);
	}

}