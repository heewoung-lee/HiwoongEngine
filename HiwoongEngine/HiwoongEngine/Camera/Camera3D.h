#pragma once

#include "Math/Vector3.h"
#include "Component/Transform3DComponent.h"
#include "Math/Matrix4x4.h"

namespace Hiwoong
{
	class Hiwoong_API Camera3D
	{
	public:

		Camera3D(Transform3DComponent& transform);

		inline Vector3 GetPosition() const { return transform.GetPosition(); }
		inline void SetTransform(Transform3DComponent& value) { transform = value; }

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
		Transform3DComponent& transform;
	};
}
