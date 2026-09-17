#include "TransformComponent.h"
#include "Math/Vector3.h"
#include "Math/MathConstants.h"
#include <cmath>

namespace Hiwoong
{

	TransformComponent::TransformComponent
	(
		Vector3 position, 
		Vector3 rotation, 
		Vector3 scale): localPosition(position), rotation(rotation), scale(scale), previousWorldPosition(position)
	{
		SetRotation(rotation);
	}

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

	void TransformComponent::SetRotation(const Vector3& newRotation)
	{
		const float fullTurn = 2.0f * MathConstants::Pi; //360도를 라디안으로 바꾼값.

		rotation = Vector3(
			std::fmod(newRotation.x, fullTurn),
			std::fmod(newRotation.y, fullTurn),
			std::fmod(newRotation.z, fullTurn)
			);
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

	Vector3 TransformComponent::GetForward() const
	{
		//물체의 최종 회전 행렬연산
		const Matrix4x4 rotationMatrix =
			Matrix4x4::RotationZ(rotation.z) *
			Matrix4x4::RotationY(rotation.y) *
			Matrix4x4::RotationX(rotation.x);

		// 기본 전방 + Z . w = 0이면 위치가 아닌 방향을 뜻함.
		const Vector4 forward =
			rotationMatrix * Vector4(0.0f, 0.0f, 1.0f, 0.0f);

		return Vector3(forward.x, forward.y, forward.z).Normalized();
	}

}