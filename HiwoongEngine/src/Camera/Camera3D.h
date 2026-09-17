#pragma once

#include "Math/Vector3.h"
#include "Component/TransformComponent.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"

namespace Hiwoong
{
	class Hiwoong_API Camera3D
	{
	public:

		Camera3D(TransformComponent& transform, float fieldOfView);


		// 카메라가 사용하는 세로 시야각을 반환한다. 단위는 라디안.
		inline float GetFieldOfView() const { return fieldOfView; }

		inline Vector3 GetPosition() const { return transform.GetLocalPosition(); }
		inline void SetTransform(TransformComponent& value) { transform = value; }

		inline void SetYaw(float value)
		{
			Vector3 rotation = transform.GetRotation();
			transform.SetRotation(Vector3(rotation.x,value,rotation.z));
		}
		inline float GetYaw() const { return transform.GetRotation().y; }
	

		Vector3 GetForward();
		Vector3 GetRight();
		Vector3 GetDown();

		Matrix4x4 GetViewMatrix();
		Vector3 ScreenPointToDirection(
			const Vector2& screenPosition,
			const Vector2& screenSize,
			const Vector2& characterSize
		);

	private:
		TransformComponent& transform;
		float fieldOfView;
	};
}
