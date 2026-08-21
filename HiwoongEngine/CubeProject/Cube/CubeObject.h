#pragma once

#include "GameObject/GameObject.h"
#include "Render/Mesh.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"

namespace Hiwoong
{
	class CubeRotationComponent;

	class CubeObject : public GameObject
	{
		TYPE_DECALRATIONS(CubeObject,GameObject)


	public:
		CubeObject();
		void Draw() override;
		void Start() override;

		char GetFaceCharacter(
			const Vector3& localNormal,
			const Matrix4x4& rotationMatrix,
			const Vector3& lightDirection
		) const;
		
	private:
		Mesh mesh;
		void SubmitSurfacePoint(
			const Vector3& localPosition,
			const Matrix4x4& mvp,
			char character
		);
		std::weak_ptr<CubeRotationComponent> rotationComponent;
		char GetShadeCharacter(float brightness) const;
	};
}

