#include "GameObject.h"
#include "Engine/Engine.h"
#include "Scene/Scene.h"

namespace Hiwoong
{

	GameObject::GameObject()
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


}