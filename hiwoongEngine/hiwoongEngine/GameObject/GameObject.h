#pragma once

#include <memory>

namespace Hiwoong
{
	//front 

	class Scene;

	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		//Intialize Object
		virtual void Start();
		//Update frame
		virtual void Update(double deltaTime);
		//Renderfing
		virtual void Draw();

		void Destroy();

		void QuitGame();


		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }


		std::shared_ptr<Scene> GetOnwer();
		void SetOwner(std::weak_ptr<Scene> newOwner);


	protected:

		// check process beginplay event
		bool hasBeganPlay = false;

		// is obejct active?
		bool isActive = true;

		// reservation removing object 
		bool hasExpired = false;

		//onwership
		std::weak_ptr<Scene> owner;

	};

}

