#include "TransformComponent.h"


namespace Hiwoong
{
	TransformComponent::TransformComponent(const Vector2& localPosition) : 
		localPosition(localPosition),previousWorldPosition(localPosition)
	{
	}
	void TransformComponent::SavePreviousWorldPosition()
	{
		previousWorldPosition = GetWorldPosition();
	}
	Vector2 TransformComponent::GetWorldPosition() const
	{
		//Parent Transform
		std::shared_ptr<TransformComponent> parentTransform = parent.lock();

		if (parentTransform == nullptr)
		{
			return localPosition;
		}
		else
		{
			return parentTransform->GetWorldPosition() + localPosition;
		}
	}
	void TransformComponent::SetWorldPosition(const Vector2& newPosition)
	{
		//Parent Transform
		std::shared_ptr<TransformComponent> parentTransform = parent.lock();

		if (parentTransform == nullptr)
		{
			localPosition = newPosition;
			return;
		}
		else
		{
			localPosition = newPosition - parentTransform->GetWorldPosition();
		}
	}
}