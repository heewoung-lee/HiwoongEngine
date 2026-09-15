#include "CrossHairMoveComponent.h"
#include "Core/Input.h"
#include "Component/TransformComponent.h"
#include "GameObject/GameObject.h"

namespace Hiwoong
{
	CrossHairMoveComponent::CrossHairMoveComponent(float minY, float maxY): minY(minY),maxY(maxY)
	{
	}
	void CrossHairMoveComponent::Update(double deltaTime)
	{
		super::Update(deltaTime);

		const auto transform = GetComponent<TransformComponent>();
		if (transform == nullptr) return;

		Vector3 position = transform->GetLocalPosition();
		position.y += Input::Get().GetMouseDeltaY() * sensitivity;//이동량 * 민감도

		if (position.y < minY) position.y = minY;
		else if (position.y > maxY) position.y = maxY;

		transform->SetLocalPosition(position);
	}
}

