#include "CubeRotationComponent.h"


namespace Hiwoong
{
	void CubeRotationComponent::Update(double deltaTime)
	{
		Component::Update(deltaTime);

		rotation =
			rotation +
			rotationSpeed * static_cast<float>(deltaTime);
	}
}

