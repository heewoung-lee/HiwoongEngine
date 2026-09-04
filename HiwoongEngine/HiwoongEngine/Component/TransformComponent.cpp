#include "TransformComponent.h"
#include "Math/Vector3.h"


namespace Hiwoong
{

	TransformComponent::TransformComponent
	(
		Vector3 position, 
		Vector3 rotation, 
		Vector3 scale): localPosition(position), rotation(rotation), scale(scale), previousWorldPosition(position){}

	void TransformComponent::SavePreviousWorldPosition()
	{
		previousWorldPosition = GetWorldPosition();
	}


	Vector3 TransformComponent::GetWorldPosition() const
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
	void TransformComponent::SetWorldPosition(const Vector3& newPosition)
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

	Matrix4x4 TransformComponent::GetModelMatrix() const
	{
		const Matrix4x4 rotationMatrix =
			Matrix4x4::RotationZ(rotation.z) *
			Matrix4x4::RotationY(rotation.y) *
			Matrix4x4::RotationX(rotation.x);

		return Matrix4x4::Translation(GetWorldPosition()) *
			rotationMatrix *
			Matrix4x4::Scale(scale);
	}

}