#include "CubeRotationComponent.h"
#include <cassert>
#include "Component/Transform3DComponent.h"
#include "GameObject/GameObject.h"


namespace Hiwoong
{
	void CubeRotationComponent::Start()
	{
		super::Start();
		std::shared_ptr<GameObject> owner = GetOwner();
		assert(owner != nullptr);

		transform3DComponent = owner->GetComponent<Transform3DComponent>();

		assert(transform3DComponent.expired() == false);
	}
	void CubeRotationComponent::Update(double deltaTime)
	{
		Component::Update(deltaTime);

		std::shared_ptr<Transform3DComponent> transform =
			transform3DComponent.lock();

		assert(transform != nullptr);

		Vector3 rotation = transform->GetRotation();

		rotation =
			rotation +
			rotationSpeed * static_cast<float>(deltaTime);

		transform->SetRotation(rotation);
	}
}

