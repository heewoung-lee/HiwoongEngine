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
		SpawnNextModule();
		std::shared_ptr<Scene> scene = GetOwner();
	}

	void SpawnManager::SpawnNextModule()
	{
		std::shared_ptr<Scene> scene = GetOwner();

		assert(scene != nullptr);

		const Vector2 spawnPosition = GetPosition();

		int number = Util::RandomRange(0, 6);

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
		newModule->AddOnLocked([this]()
			{
				SpawnNextModule();
			});
	}

}