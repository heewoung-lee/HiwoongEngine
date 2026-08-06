#pragma once

#include "Core/Core.h"
#include "Core/HiwoongObject.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <memory>
#include <string>

namespace Hiwoong
{
	//front 

	class Scene;

	class Hiwoong_API GameObject : public HiwoongObject
	{
		// add Macro
		TYPE_DECALRATIONS(GameObject,HiwoongObject)

	public:
		GameObject(
			const std::string& image = "",
			const Vector2& position = Vector2::Zero,
			Color color = Color::White
		);
		virtual ~GameObject();

		//Intialize Object
		virtual void Start();
		//Update frame
		virtual void Update(double deltaTime);
		//Renderfing
		virtual void Draw();

		void Destroy();

		void QuitGame();

		//Getter / Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		std::shared_ptr<Scene> GetOnwer();
		void SetOwner(std::weak_ptr<Scene> newOwner);

		inline Vector2 GetPosition() const { return position; }
		void SetPosition(const Vector2& newPosition);


	protected:
		// check process beginplay event
		bool hasBeganPlay = false;

		// is obejct active?
		bool isActive = true;

		// reservation removing object 
		bool hasExpired = false;

		//onwership
		std::weak_ptr<Scene> owner;

		std::string image;

		Color color = Color::White;

		//GameObject Width
		int width = 0;

		int sortingOrder = 0;

		Vector2 position;
	};

}

