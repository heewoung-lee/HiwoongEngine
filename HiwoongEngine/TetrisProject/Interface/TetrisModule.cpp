#include "TetrisModule.h"
#include "GameObject/TetrisBlocks/Block.h"
#include "Scene/Scene.h"
#include "GameObject/Player/PlayerInputComponent.h"
#include "Scene/TestScene.h"
#include "Component/Component.h"
#include <memory>
#include <cassert>
namespace Hiwoong
{
	void TetrisModule::AddOnLocked(LockedCallback callback)
	{
		if (callback == nullptr)
		{
			return;
		}


		onLockedCallbacks.emplace_back(std::move(callback));

	}
	bool TetrisModule::CanMove(const Vector2& direction) const
	{
		if (isLocked)
		{
			return false;
		}

		std::shared_ptr<TestScene> scene = std::dynamic_pointer_cast<TestScene>(GetOwner());

		assert(scene != nullptr);

		TetrisBoard* board = scene->GetBoard();

		assert(board != nullptr);


		for (const auto& localposition : GetBlockPosition())
		{
			Vector2 nextPosition = GetWorldPosition() + localposition + direction;

			if (board->IsOccupied(nextPosition))
			{
				return false;
			}
		}
		return true;
	}

	bool TetrisModule::TryMove(const Vector2& direction)
	{
		if (CanMove(direction) == false)
		{
			return false;
		}
		SetPosition(GetPosition() + direction);
		return true;
	}


	void TetrisModule::Start()
	{
		GameObject::Start();
		AddComponent<PlayerInputComponent>();

		Color currentColor = GetColor();
		std::shared_ptr<Scene> currentScene = GetOwner();

		assert(currentScene != nullptr);

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

		const Vector2 Down(0, 1);

		if (TryMove(Down) == false)
		{
			Lock();
		}

	}

	void TetrisModule::Lock()
	{
		if (isLocked)
		{
			return;
		}
		//find Board in Scene 
		std::shared_ptr<TestScene> scene = std::dynamic_pointer_cast<TestScene>(GetOwner());
		
		assert(scene != nullptr);

		TetrisBoard* board = scene->GetBoard();

		assert(board != nullptr);

		//Find Block's worldPositon and blocks position in local + world
		const Vector2 moduleWorldPosition = GetWorldPosition();
		
		for (const Vector2& localPosition : GetBlockPosition())
		{
			const Vector2 blockWorldPosition = moduleWorldPosition + localPosition;

			board->SetOccupied(blockWorldPosition, true);
		}
		BroadcastOnLocked();
		isLocked = true;
	}

	void TetrisModule::BroadcastOnLocked()
	{
		for (LockedCallback& callback : onLockedCallbacks)
		{
			if (callback)
			{
				callback();
			}
		}
	}

}