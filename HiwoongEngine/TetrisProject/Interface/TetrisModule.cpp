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
			Vector3 worldPos = GetWorldPosition();


			Vector2 nextPosition = Vector2(worldPos.x, worldPos.y) + localposition + direction;

			if (board->IsOccupied(nextPosition))
			{
				return false;
			}
		}
		return true;
	}
	void TetrisModule::Drop()
	{
		if (isLocked)
		{
			return;
		}

		const Vector2 down(0, 1);

		while (TryMove(down))
		{
		}

		Lock();
	}
	bool TetrisModule::TryMove(const Vector2& direction)
	{
		if (CanMove(direction) == false)
		{
			return false;
		}

		SetPosition(GetPosition() + Vector3(direction.x,direction.y,0));
		return true;
	}

	bool TetrisModule::TryRotate()
	{
		if (isLocked || canRotate == false)
		{
			return false;
		}

		//rotation Position Array
		std::array<Vector2, 4> rotatedPositions = blockPositions;

		const Vector2 pivot = rotationPivot;

		for (std::size_t index = 0; index < blockPositions.size();++index)
		{
			const Vector2 relativePosition = blockPositions[index] - pivot;

			rotatedPositions[index] = Vector2(
				pivot.x - relativePosition.y,
				pivot.y + relativePosition.x
			);
		}

		std::shared_ptr<TestScene> scene = std::dynamic_pointer_cast<TestScene>(GetOwner());

		assert(scene != nullptr);

		TetrisBoard* board = scene->GetBoard();

		assert(board != nullptr);

		const Vector3 position = GetWorldPosition();

		const Vector2 moduleWorldPosition(position.x, position.y);

		for (const Vector2& rotatedPosition : rotatedPositions)
		{
			const Vector2 blockWoldPosition = moduleWorldPosition + rotatedPosition;
			
			//if the blockPosition overlaps board return false
			if (board->IsOccupied(blockWoldPosition))
			{
				return false;
			}
		}

		//if pass the rotation apply rotation
		blockPositions = rotatedPositions;
		for (std::size_t index = 0; index < blocks.size();++index)
		{
			auto block = blocks[index].lock();
			assert(block != nullptr);

			Vector2 blockPos = blockPositions[index];

			block->SetPosition(Vector3(blockPos.x,blockPos.y,0));
		}

		return true;
	}


	void TetrisModule::Start()
	{
		GameObject::Start();
		AddComponent<PlayerInputComponent>();

		Color currentColor = GetColor();
		std::shared_ptr<Scene> currentScene = GetOwner();

		assert(currentScene != nullptr);

		for(std::size_t index = 0; index < blockPositions.size(); ++index)
		{
			std::shared_ptr<Block> block = currentScene->Instantiate<Block>
			(
				blockPositions[index],
				currentColor
			);

			block->AttachTo(shared_from_this(), false);

			blocks[index] = block;
		}
		//Set drop Speed
		std::shared_ptr<TestScene> testScene =
			std::dynamic_pointer_cast<TestScene>(GetOwner());

		assert(testScene != nullptr);

		dropInterval = testScene->GetDropInterval();

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

		Vector3 curPos = GetWorldPosition();
		const Vector2 moduleWorldPosition(curPos.x, curPos.y);
		
		for (std::size_t idx = 0; idx < blocks.size();++idx)
		{
			std::shared_ptr<Block> block = blocks[idx].lock();
			assert(block != nullptr);

			Vector3 blockPos = block->GetWorldPosition();

			board->SetBlock(Vector2(
				static_cast<int>(blockPos.x),
				static_cast<int>(blockPos.y)
			), block);
		}

		board->CheckFillRow();
		isLocked = true;
		BroadcastOnLocked();
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