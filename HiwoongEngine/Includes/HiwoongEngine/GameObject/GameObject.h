#pragma once

#include "Core/Core.h"
#include "Core/HiwoongObject.h"
#include "Component/Component.h"
#include "Component/TransformComponent.h"
#include "Math/Vector2.h"
#include "Math/Color.h"
#include <memory>
#include <string>
#include <type_traits> //is_base_of
#include <utility> //std::forward
#include <vector>


namespace Hiwoong
{
	//front 

	class Scene;

	class Hiwoong_API GameObject : public HiwoongObject, public std::enable_shared_from_this<GameObject>
	{
		// declaration friend class to aceess
		friend class Scene;

		// add Macro
		TYPE_DECALRATIONS(GameObject, HiwoongObject)

	public:

		GameObject(const Vector2& position = Vector2::Zero);

		GameObject(
			const std::string& image,
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

		void OnCollision(const std::shared_ptr<GameObject>& other);

		void SavePreviousState();

		// Attached Component to GameObejct
		template<typename T, typename ...Args,
			typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
			std::shared_ptr<T> AddComponent(Args&& ...args)
		{
			// Check Transform
			assert(!std::is_same<T, TransformComponent>::value, "TransformComponent is created by an GameOjbect");

			//add new Component
			std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>...);

			addRequestedComponentList.emplace_back(newComponent);

			return newComponent;
		}

		// find component in GameObject
		template<typename T,
			typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
			std::shared_ptr<T> GetComponent() const
		{
			for (const std::shared_ptr<Component>& component : componentList)
			{
				if (component && component->IsTypeOf<T>())
				{
					return std::static_pointer_cast<T>(component);
				}
			}

			for (const std::shared_ptr<Component>& component : addRequestedComponentList)
			{
				if (component && component->IsTypeOf<T>())
				{
					return std::static_pointer_cast<T>(component);
				}
			}

			return nullptr;
		}

		//Getter / Setter
		inline bool HasBeganPlay() const { return hasBeganPlay; }
		inline bool IsActive() const { return isActive && !hasExpired; }
		inline bool HasExpired() const { return hasExpired; }

		std::shared_ptr<Scene> GetOnwer();
		void SetOwner(std::weak_ptr<Scene> newOwner);

		Vector2 GetPosition() const;
		Vector2 GetWorldPosition() const;

		void SetPosition(const Vector2& newPosition);

		//return previous Position
		Vector2 GetPreviousPosition() const;
		
		inline int GetWidth() const { return width; }
		inline std::shared_ptr<TransformComponent> GetWorldTransform() const { return transform; }


	protected:
		void ProcessAddComponents();

		void BindComponentOwners();

	protected:
		// check process beginplay event
		bool hasBeganPlay = false;

		// is obejct active?
		bool isActive = true;

		// reservation removing object 
		bool hasExpired = false;

		//onwership
		std::weak_ptr<Scene> owner;


		//Attched TransformComponent for GameObject default
		std::shared_ptr<TransformComponent> transform;

		//list addComponents
		std::vector<std::shared_ptr<Component>> componentList;

		// list Requests AddComponent
		std::vector<std::shared_ptr<Component>> addRequestedComponentList;

		std::string image;

		Color color = Color::White;

		//GameObject Width
		int width = 0;

		int sortingOrder = 0;

		Vector2 position;

		Vector2 previousPosition;
	};

}

