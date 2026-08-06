#include "GameObject.h"
#include "Engine/Engine.h"
#include "Scene/Scene.h"
#include "Render/Renderer.h"

namespace Hiwoong
{

	GameObject::GameObject(
		const std::string& image,
		const Vector2& position,
		Color color) : image(image), position(position), color(color), width(static_cast<int>(image.length()))
	{

	}
	GameObject::~GameObject()
	{

	}

	//Intialize Object
	void GameObject::Start()
	{

		hasBeganPlay = true;
	}
	//Update frame
	void GameObject::Update(double deltaTime)
	{

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

	std::shared_ptr<Scene> GameObject::GetOnwer()
	{
		return owner.lock();
	}

	void GameObject::SetOwner(std::weak_ptr<Scene> newOwner)
	{
		owner = newOwner;
	}

	void GameObject::SetPosition(const Vector2& newPosition)
	{
		if (position == newPosition) return;

		position = newPosition;
	}


}