#include "Component.h"
#include "Scene/Scene.h"
#include "GameObject/GameObject.h"

namespace Hiwoong
{
	Component::Component()
	{

	}

	Component::~Component()
	{

	}

	void Component::Start()
	{
		hasStarted = true;
	}
	void  Component::Update(double deltaTime)
	{

	}
	void  Component::Draw()
	{

	}
	void Component::OnCollision(const std::shared_ptr<GameObject>& other)
	{

	}
	std::shared_ptr<Scene> Component::GetScene() const
	{
		std::shared_ptr<GameObject> ownerObject = owner.lock();

		if (ownerObject == nullptr) return nullptr;
		
		return ownerObject->GetOwner();
	}
}