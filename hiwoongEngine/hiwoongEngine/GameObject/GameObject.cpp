#include "GameObject.h"
#include "Engine/Engine.h"
#include "Scene/Scene.h"
#include "Render/Renderer.h"

namespace Hiwoong
{
	GameObject::GameObject(const Vector2& position) : GameObject("",position,Color::White)
	{
	}
	GameObject::GameObject(
		const std::string& image,
		const Vector2& position,
		Color color) : image(image), position(position), color(color), width(static_cast<int>(image.length()))
	{
		//AddComponent Transform

		transform = std::make_shared<TransformComponent>(position);
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
		Renderer::Get().Submit(image, position, color, sortingOrder);

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

	std::shared_ptr<Scene> GameObject::GetOnwer()
	{
		return owner.lock();
	}

	void GameObject::SetOwner(std::weak_ptr<Scene> newOwner)
	{
		owner = newOwner;
		
		//changing ownership
		BindComponentOwners();
	}

	Vector2 GameObject::GetPosition() const
	{
		// return transform position
		return transform ? transform->GetLocalPosition() : Vector2::Zero;
	}
	Vector2 GameObject::GetWorldPosition() const
	{
		return transform ? transform->GetWorldPosition() : Vector2::Zero;
	}

	void GameObject::SetPosition(const Vector2& newPosition)
	{
		if (GetPosition() == newPosition) return;

		//position = newPosition;

		if (transform != nullptr)
		{
			transform->SetLocalPosition(newPosition);
		}
	}


	Vector2 GameObject::GetPreviousPosition() const
	{
		// return previous frame position which manged by transform.
		return transform ? transform->GetPreviousWorldPosition() : Vector2::Zero;
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