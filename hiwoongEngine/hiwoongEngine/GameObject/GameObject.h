#pragma once

#include "Core/Core.h"
#include "Core/HiwoongObject.h"
#include "Component/Component.h"
#include "Component/TransformComponent.h"
#include "Math/Vector3.h"
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

		GameObject(const Vector3& position = Vector3::Zero);
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

		//Select ParentGameobject in Scene Graph
		void AttachTo(
			const std::shared_ptr<GameObject>& newParent,
			bool keepWorldPosition = true);

		void DetachFromParent();



		// Attached Component to GameObejct
		template<typename T, typename ...Args,
			typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
			std::shared_ptr<T> AddComponent(Args&& ...args)
		{
			// Check Transform
			static_assert(!std::is_same<T, TransformComponent>::value, 
				"Do not call AddComponent<TransformComponent>().Use GetTransform() instead.");

			//add new Component
			std::shared_ptr<T> newComponent = std::make_shared<T>(std::forward<Args>(args)...);

			addRequestedComponentList.emplace_back(newComponent);

			return newComponent;
		}

		// find component in GameObject
		template<typename T,
			typename = std::enable_if_t<std::is_base_of<Component, T>::value>>
			std::shared_ptr<T> GetComponent() const
		{
			//트랜스폼은 트랜스폼 전용자리가 있기에 찾는게 트랜스폼이라면 Transform반환
			if constexpr (std::is_same_v<T, TransformComponent>)
			{
				return transform;
			}

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

		std::shared_ptr<Scene> GetOwner() const;
		void SetOwner(std::weak_ptr<Scene> newOwner);

		Vector3 GetPosition() const;
		Vector3 GetWorldPosition() const;

		void SetPosition(const Vector3& newPosition);

		//return previous Position
		Vector3 GetPreviousPosition() const;
		
		inline int GetWidth() const { return width; }
		
		// return Parent GameObject 
		inline std::shared_ptr<GameObject> GetParent() const { return parent.lock(); }

		// return child GameObject
		inline const std::vector<std::weak_ptr<GameObject>>& GetChildren() const { return children; }


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

		//Parent GameObject in Scene Graph
		std::weak_ptr<GameObject> parent;

		//Child Objects in Scene Graph
		std::vector<std::weak_ptr<GameObject>> children;

		std::string image;

		Color color = Color::White;

		//GameObject Width
		int width = 0;

		int sortingOrder = 0;
	};

}

