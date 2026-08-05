#include "Scene.h"

namespace Hiwoong
{
	Scene::Scene()
	{

	}
	Scene::~Scene()
	{

	}

	// Initialize Level
	void Scene::SceneInitialize()
	{
		//Setting Flag Initialize Level
		hasInitialzed = true;
	}

	//Frame Event Function
	void Scene::Start()
	{
		//initialze actor when called
		for (std::shared_ptr<GameObject>& actor : gameObjectList)
		{
			if (actor->HasBeganPlay() == true)
			{
				//actor already initialized
				continue;
			}

			// actor hasn't been initialzed start init
			actor->Start();
		}


	}
	void Scene::Update(double deltaTime)
	{
		for (std::shared_ptr<GameObject> actor : gameObjectList)
		{
			if (actor->IsActive() == false)
			{
				continue;
			}
			actor->Update(deltaTime);
		}
	}
	void Scene::Draw()
	{
		for (std::shared_ptr<GameObject> actor : gameObjectList)
		{
			if (actor->IsActive() == false)
			{
				continue;
			}
			actor->Draw();
		}
	}
	void Scene::ProcessAddAndDestoryActors()
	{
		//Process Actor delete.
		for (auto iterator = gameObjectList.begin(); iterator != gameObjectList.end();)
		{
			//Check oper to delete actor
			if ((*iterator)->HasExpired())
			{
				iterator = gameObjectList.erase(iterator);
				continue;
			}

			++iterator;
		}


		// return when the addrequtestList is empty
		if (addRequestedGameObjectList.empty() == true)
		{
			return;
		}

		// process request adding Actor
		for (const std::shared_ptr<GameObject>& actor : addRequestedGameObjectList)
		{
			gameObjectList.emplace_back(actor);
		}

		addRequestedGameObjectList.clear();
	}
}