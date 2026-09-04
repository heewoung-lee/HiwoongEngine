#pragma once

#include "GameObject/GameObject.h"
#include "Render/Mesh.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Component/TransformComponent.h"

namespace Hiwoong
{
	class TransformComponent;

	class CubeObject : public GameObject
	{
		TYPE_DECALRATIONS(CubeObject,GameObject)


	public:
		CubeObject(
			const Vector3& position,
			const Vector3& rotation,
			const Vector3& scale);
		void Draw() override;
		void Start() override;

		char GetFaceCharacter(
			const Vector3& localNormal,
			const Matrix4x4& rotationMatrix,
			const Vector3& lightDirection
		) const;
		
	private:
		Mesh mesh;
		std::weak_ptr<TransformComponent> transform3DComponent;
		char GetShadeCharacter(float brightness) const;

	};
}

