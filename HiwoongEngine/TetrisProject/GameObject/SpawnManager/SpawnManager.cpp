#include "SpawnManager.h"
#include "Scene/Scene.h"
#include "Util/Util.h"
#include "GameObject/TetrisBlocks/TetrisModule.h"
#include "GameObject/BackGround/TetrisBoard.h"
#include "SCene/TestScene.h"
#include "Manager/TetrisGameState.h"


#include <cassert>

namespace Hiwoong
{

	SpawnManager::SpawnManager(const Vector2& position) : GameObject(position)
	{

	}

	void SpawnManager::Start()
	{
		super::Start();

		//Spawn Block
		//if Scene is not GameOver And there is not block which I controled
		//spawn

		TetrisGameState& state = TetrisGameState::Get();
		//it is first spawn Block
		if (state.GetNextModuleNumber() < 0)
		{
			state.SetNextModuleNumber(
				Util::RandomRange(0, 6)
			);
		}
		SpawnNextModule();
	}

	void SpawnManager::SpawnNextModule()
	{
		std::shared_ptr<Scene> scene = GetOwner();

		assert(scene != nullptr);

		const Vector2 spawnPosition = GetPosition();

		TetrisGameState& state = TetrisGameState::Get();

		const int number = state.GetNextModuleNumber();

		std::shared_ptr<TetrisModule> newModule;

		switch (number)
		{
		case 0: newModule = scene->Instantiate<IModule>(spawnPosition); break;
		case 1: newModule = scene->Instantiate<OModule>(spawnPosition); break;
		case 2: newModule = scene->Instantiate<TModule>(spawnPosition); break;
		case 3: newModule = scene->Instantiate<LModule>(spawnPosition); break;
		case 4: newModule = scene->Instantiate<JModule>(spawnPosition); break;
		case 5: newModule = scene->Instantiate<SModule>(spawnPosition); break;
		case 6: newModule = scene->Instantiate<ZModule>(spawnPosition); break;
		}
		assert(newModule != nullptr);


		std::shared_ptr<TestScene> testScene =
			std::dynamic_pointer_cast<TestScene>(scene);

		assert(testScene != nullptr);

		TetrisBoard* board = testScene->GetBoard();

		assert(board != nullptr);

		//Setting GameOver
		for (const Vector2& position : newModule->GetBlockPosition())
		{
			if (board->IsOccupied(position + newModule->GetWorldPosition()) == true)
			{
				gameOver = true;
				newModule->Destroy();
				return;
			}
		}

		newModule->AddOnLocked([this]()
			{

				// Check board fill horizontal
				std::shared_ptr<TestScene> scene =
					std::dynamic_pointer_cast<TestScene>(GetOwner());

				assert(scene != nullptr);

				TetrisBoard* board = scene->GetBoard();
				assert(board != nullptr);

				SpawnNextModule();
			});

		//Next ModuleNumber
		state.SetNextModuleNumber(
			Util::RandomRange(0, 6)
		);


	}

}