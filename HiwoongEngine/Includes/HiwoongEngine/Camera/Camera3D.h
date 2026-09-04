#pragma once

#include "Math/Vector3.h"
#include "Component/TransformComponent.h"
#include "Math/Matrix4x4.h"

namespace Hiwoong
{
	class Hiwoong_API Camera3D
	{
	public:

		Camera3D(TransformComponent& transform);

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


	private:
		TransformComponent& transform;
	};
}
