#pragma once


#include "Engine/Engine.h"
#include "Core/Core.h"
#include "Component.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

namespace Hiwoong
{
	class Hiwoong_API Transform3DComponent : public Component
	{
		TYPE_DECALRATIONS(Transform3DComponent, Component)

	public:
		Transform3DComponent(
			Vector3 position = Vector3::Zero,
			Vector3 rotation = Vector3::Zero,
			Vector3 scale = Vector3(1,1,1));
		~Transform3DComponent() = default;

		inline const Vector3& GetPosition() const{ return position; }
		inline const Vector3& GetRotation() const { return rotation; }
		inline const Vector3& GetScale() const { return scale; }

		inline void SetPosition(const Vector3& newPosition) { position = newPosition; }
		inline void SetRotation(const Vector3& newRotation) { rotation = newRotation; }
		inline void SetScale(const Vector3& newScale) { scale = newScale; }

		Matrix4x4 GetModelMatrix() const;
	private:
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
	};

}



