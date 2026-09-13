#include "MouseLookComponent.h"
#include "Core/Input.h"
#include "GameObject/GameObject.h"
#include "Component/TransformComponent.h"

namespace Hiwoong
{
	void MouseLookComponent::Update(double deltatime)
	{
		super::Update(deltatime);

		const std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>();
		if (transform == nullptr) return;

		Vector3 rotation = transform->GetRotation();

		//회전은 가로 이동량 X 민감도를 계산해 더한다.
		const float mouseDeltaX = Input::Get().GetMouseDeltaX();
		rotation.y += mouseDeltaX * sensitivity;
		

		transform->SetRotation(rotation);
	}
}

