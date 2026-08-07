#include "Component.h"


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
	void  Component::OnCollision(const std::shared_ptr<GameObject>& other)
	{

	}
}