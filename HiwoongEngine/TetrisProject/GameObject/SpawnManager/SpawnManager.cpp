#include "SpawnManager.h"
#include "Scene/Scene.h"
#include "GameObject/TetrisBlocks/IModule.h"
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
		
		std::shared_ptr<Scene> scene = GetOnwer();

		assert(scene != nullptr);

		const Vector2 spawnPosition = GetPosition();

		//TODO:: RandomModule Spawn
		scene->Instantiate<IModule>(spawnPosition);
	}

}