#include "Camera3D.h"
#include "Math/Matrix4x4.h"
#include "Component/TransformComponent.h"
#include <cmath>

namespace Hiwoong
{
	Camera3D::Camera3D(TransformComponent& transform): transform(transform)
	{
	}

	//카메라의 전방.
	//점의 앞뒤 위치 확인
	Vector3 Camera3D::GetForward()
	{
		Vector3 rotation = transform.GetRotation();

		return Vector3(std::sin(rotation.y), 0, std::cos(rotation.y));
	}

	//카메라의 오른쪽 방향.
	//점의 좌우 위치 측정
	Vector3 Camera3D::GetRight()
	{
		Vector3 rotation = transform.GetRotation();

		return Vector3(std::cos(rotation.y), 0, std::sin(rotation.y) * -1);
	}

	//점의 위아래 위치 측정
	Vector3 Camera3D::GetDown()
	{
		return Vector3(0,1,0);
	}

	//GetView()는 현재 카메라의 위치와 방향을 View 행렬로 만들어 반환하는 함수.
	Matrix4x4 Camera3D::GetViewMatrix()
	{
		Vector3 cameraPosition = transform.GetWorldPosition();
		Vector3 lookAtPoint = cameraPosition + GetForward();
		Vector3 up(0, -1, 0);

		return Matrix4x4::LookAt(cameraPosition, lookAtPoint, up);
	}

}
