#include "GameObject.h"
#include "Engine/Engine.h"
#include "Scene/Scene.h"
#include "Render/Renderer.h"
namespace Hiwoong
{
	GameObject::GameObject(const Vector3& position)
	{
		transform = std::make_shared<TransformComponent>(position);

		//TransformComponent - > Transform3DComponent
	}
	GameObject::~GameObject()
	{

	}

	//Intialize Object
	void GameObject::Start()
	{

		hasBeganPlay = true;

		//Send Event to Component
		for (const std::shared_ptr<Component>& component : componentList)
		{
			if (component->HasStared() == false)
			{
				component->Start();
			}

		}
	}
	//Update frame
	void GameObject::Update(double deltaTime)
	{
		if (IsActive() == false)
		{
			return;
		}

		for (const std::shared_ptr<Component>& component : componentList)
		{
			component->Update(deltaTime);
		}

	}
	//Renderfing
	void GameObject::Draw()
	{
		// if it is Enabled 
		if (IsActive() == false)
		{
			return;
		}

		for (const std::shared_ptr<Component>& component : componentList)
		{
			component->Draw();
		}
	}

	void GameObject::OnCollision(const std::shared_ptr<GameObject>& other)
	{
		if (IsActive() == false)
		{
			return;
		}
		for (const std::shared_ptr<Component>& component : componentList)
		{
			component->OnCollision(other);
		}

	}

	void GameObject::Destroy()
	{
		//revservation to remove actor
		//actor will be delted next frame.
		hasExpired = true;

		for (const std::weak_ptr<GameObject>& child : children)
		{
			std::shared_ptr<GameObject> childGameObject = child.lock();
			if (childGameObject != nullptr)
			{
				childGameObject->Destroy();
			}
		}

	}

	void GameObject::QuitGame()
	{
		//Request quit Engine
		Engine::Get().Quit();
	}

	void GameObject::SavePreviousState()
	{
		//previousPosition = position;
		if (transform)
		{
			transform->SavePreviousWorldPosition();
		}
	}

	void GameObject::AttachTo(const std::shared_ptr<GameObject>& newParent, bool keepWorldPosition)
	{
		// if there are no parents and pointer that we will set for parent is this continue
		if (newParent == nullptr || newParent.get() == this ) return;

		//delete previsous parents
		DetachFromParent();

		parent = newParent;
		newParent->children.emplace_back(weak_from_this());

		if (transform && newParent->GetComponent<TransformComponent>())
		{
			Vector3 worldPosition = transform->GetWorldPosition();

			transform->SetParent(newParent->GetComponent<TransformComponent>());

			if (keepWorldPosition)
			{
				transform->SetWorldPosition(worldPosition);
			}
		}

	}

	void GameObject::DetachFromParent()
	{
		std::shared_ptr<GameObject> oldParent = parent.lock();

		if (oldParent)
		{
			//Load previos Chilren List
			auto& siblingList = oldParent->children;

			for (auto iterator = siblingList.begin(); iterator != siblingList.end();++iterator)
			{
				//Find me from list of previous parent 
				if ((*iterator).lock().get() == this)
				{
					siblingList.erase(iterator);
					break;
				}
			}

		}

		// initialzie 
		parent.reset();

		if (transform)
		{
			Vector3 worldPosition = transform->GetWorldPosition();
			transform->SetParent(std::weak_ptr<TransformComponent>());
			transform->SetWorldPosition(worldPosition);
		}
	}

	std::shared_ptr<Scene> GameObject::GetOwner() const
	{
		return owner.lock();
	}

	void GameObject::SetOwner(std::weak_ptr<Scene> newOwner)
	{
		owner = newOwner;
		
		//changing ownership
		BindComponentOwners();
	}

	Vector3 GameObject::GetPosition() const
	{
		// return transform position
		return transform ? transform->GetLocalPosition() : Vector3::Zero;
	}
	Vector3 GameObject::GetWorldPosition() const
	{
		return transform ? transform->GetWorldPosition() : Vector3::Zero;
	}

	void GameObject::SetPosition(const Vector3& newPosition)
	{
		if (GetPosition() == newPosition) return;

		//position = newPosition;

		if (transform != nullptr)
		{
			transform->SetLocalPosition(newPosition);
		}
	}


	Vector3 GameObject::GetPreviousPosition() const
	{
		// return previous frame position which manged by transform.
		return transform ? transform->GetPreviousWorldPosition() : Vector3::Zero;
	}

	void GameObject::ProcessAddComponents()
	{
		if (addRequestedComponentList.empty()) return;

		// Setting ownership
		BindComponentOwners();

		for (const std::shared_ptr<Component>& component : addRequestedComponentList)
		{
			if (component == nullptr) continue;

			componentList.emplace_back(component);

			if (hasBeganPlay && !component->HasStared())
			{
				component->Start();
			}
		}

		addRequestedComponentList.clear();
	}

	void GameObject::BindComponentOwners()
	{
		std::shared_ptr<GameObject> gameObject = shared_from_this();
		if (gameObject == nullptr) return;

		if (transform != nullptr)
		{
			transform->SetOwner(gameObject);
		}

		//setting ownership
		for (const std::shared_ptr<Component>& component : componentList)
		{
			if (component == nullptr) continue;

			component->SetOwner(gameObject);
		}

		for (const std::shared_ptr<Component>& component : addRequestedComponentList)
		{
			if (component == nullptr) continue;

			component->SetOwner(gameObject);
		}

	}


}