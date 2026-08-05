#pragma once

#include "GameObject/GameObject.h"
#include "Core/Core.h"
#include <type_traits>
#include <utility>
#include <vector>
#include <memory>


namespace Hiwoong
{
	// Scene Class
	// Manages GameObjects in this Scene
	// Send frame Event to Actor
	// Provide some funtional setting.
	class Hiwoong_API Scene : public std::enable_shared_from_this<Scene>
	{
		// declaraion Frind class Engine 
		friend class Engine;


	public:
		Scene();
		virtual ~Scene();

		// Initialize Level
		virtual void SceneInitialize();

		//Frame Event Function
		virtual void Start();
		virtual void Update(double deltaTime);
		virtual void Draw();

		// template of game object spawner
		template<typename T, typename ...Args,
			typename = std::enable_if_t<std::is_base_of<GameObject,T>::value>>
			std::shared_ptr<T> Instantiate(Args&&... args)
		{
			// Instantiate GameObject
			std::shared_ptr<T> newGameObject = 
				std::make_shared<T>(std::forward<Args>(args)...);


			addRequestedGameObjectList.emplace_back(newGameObject);

			newGameObject->SetOwner(shared_from_this());


			return newGameObject;
		}


		//Getter.
		inline bool HasInitialized() const {return hasInitialzed;}

	protected:
		void ProcessAddAndDestoryActors();

	protected:
		// flag for level's initialize
		bool hasInitialzed = false;

		// Actors in the Level
		std::vector<std::shared_ptr<GameObject>> gameObjectList;

		// requested adding actorList
		std::vector<std::shared_ptr<GameObject>> addRequestedGameObjectList;
	};


}
