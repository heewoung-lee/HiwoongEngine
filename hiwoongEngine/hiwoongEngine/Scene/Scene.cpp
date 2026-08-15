#include "Scene.h"
#include "GameObject/GameObject.h"
#include "Engine/Engine.h"
#include "Math/Vector2.h"


namespace Hiwoong
{
	Scene::Scene(Vector2 screenSize) : screenSize(screenSize)
	{
		if (this->screenSize == Vector2::Zero)
		{
			this->screenSize = Vector2(
				Engine::Get().GetWidth(),
				Engine::Get().GetHeight()
			);
		}
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
		for (std::shared_ptr<GameObject> gameObject : gameObjectList)
		{
			if (gameObject->IsActive() == false)
			{
				continue;
			}
			gameObject->Draw();
		}
	}
	void Scene::ProcessAddAndDestoryActors()
	{
		for (const std::shared_ptr<GameObject>& gameObejct : gameObjectList)
		{
			if (gameObejct == nullptr) continue;

			gameObejct->ProcessAddComponents();
		}


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