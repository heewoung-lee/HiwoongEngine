#include "SpawnManager.h"
#include "Scene/Scene.h"
#include "Util/Util.h"
#include "GameObject/TetrisBlocks/TetrisModule.h"


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

		int number = Util::RandomRange(0, 6);

		switch (number)
		{
			case 0: scene->Instantiate<IModule>(spawnPosition); break;
			case 1: scene->Instantiate<OModule>(spawnPosition); break;
			case 2: scene->Instantiate<TModule>(spawnPosition); break;
			case 3: scene->Instantiate<LModule>(spawnPosition); break;
			case 4: scene->Instantiate<JModule>(spawnPosition); break;
			case 5: scene->Instantiate<SModule>(spawnPosition); break;
			case 6: scene->Instantiate<ZModule>(spawnPosition); break;
		}
	}

}