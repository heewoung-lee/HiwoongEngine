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
		return localPosition;
	}
	void TransformComponent::SetWorldPosition(const Vector2& newPosition)
	{
		localPosition = newPosition;
	}
}