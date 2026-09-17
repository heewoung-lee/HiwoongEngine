#include "Camera3D.h"
#include "Math/Matrix4x4.h"
#include "Component/TransformComponent.h"
#include "Math/MathConstants.h"
#include <cassert>
#include <cmath>

namespace Hiwoong
{
	Camera3D::Camera3D(
		TransformComponent& transform,
		float fieldOfView)
		: transform(transform),
		fieldOfView(fieldOfView)
	{
		// 시야각은 0도 초과, 180도 미만이어야 한다.
		assert(fieldOfView > 0.0f &&
			fieldOfView < MathConstants::Pi);
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

	Vector3 Camera3D::ScreenPointToDirection(
		const Vector2& screenPosition,
		const Vector2& screenSize,
		const Vector2& characterSize)
	{
		assert(screenSize.x > 1 && screenSize.y > 1);
		assert(characterSize.x > 0 && characterSize.y > 0);

		const float width = static_cast<float>(screenSize.x);
		const float height = static_cast<float>(screenSize.y);

		// 화면 좌표를 -1 ~ +1 범위로 변환
		// 왼쪽·위쪽은 -1, 가운데는 0, 오른쪽·아래쪽은 +1.
		// 기존 NdcToScreen()의 계산을 거꾸로 적용한다.
		const float ndcX =
			2.0f * static_cast<float>(screenPosition.x) /
			(width - 1.0f) - 1.0f;

		const float ndcY =
			2.0f * static_cast<float>(screenPosition.y) /
			(height - 1.0f) - 1.0f;

		// 문자 한 칸의 실제 가로·세로 크기까지 반영
		// 화면을 렌더링할 때 사용하는 비율과 같아야함
		const float aspectRatio =
			(width / height) *
			(static_cast<float>(characterSize.x) /
				static_cast<float>(characterSize.y));

		// 카메라 앞쪽 거리 1에서 보이는 화면 높이의 절반.
		const float halfHeight = std::tan(fieldOfView * 0.5f);
		const float halfWidth = halfHeight * aspectRatio;

		// 정면 방향에 조준점의 좌우·상하 편차를 더한다.
		const Vector3 direction =
			GetForward() +
			GetRight() * (ndcX * halfWidth) +
			GetDown() * (ndcY * halfHeight);

		// 조준 위치와 관계없이 탄환 속도가 일정하도록 길이를 1로 만든다.
		return direction.Normalized();
	}

}
