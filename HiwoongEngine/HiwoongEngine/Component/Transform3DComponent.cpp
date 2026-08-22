#include "Transform3DComponent.h"


namespace Hiwoong
{
	Transform3DComponent::Transform3DComponent(
		Vector3 position,
		Vector3 rotation,
		Vector3 scale) : position(position), rotation(rotation), scale(scale){}

    Matrix4x4 Transform3DComponent::GetModelMatrix() const
    {
        const Matrix4x4 rotationMatrix =
            Matrix4x4::RotationZ(rotation.z) *
            Matrix4x4::RotationY(rotation.y) *
            Matrix4x4::RotationX(rotation.x);

        return Matrix4x4::Translation(position) *
            rotationMatrix *
            Matrix4x4::Scale(scale);
    }

}
