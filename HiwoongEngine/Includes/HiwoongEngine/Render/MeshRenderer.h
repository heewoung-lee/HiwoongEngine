#pragma once
#include "Core/Core.h"
#include "Render/Mesh.h"
#include "Math/Matrix4x4.h"
#include "Render/RenderView.h"

namespace Hiwoong
{
	class Hiwoong_API MeshRenderer
	{

	public : 
		//메쉬를 모델->뷰->프로젝션 행렬순으로 계산한뒤 렌더링
		void Render
		(
			const Mesh& mesh,
			const Matrix4x4& model,
			const RenderView& renderView
		) const;


		//삼각형 자르기
		static std::vector<Vector3> ClipTriangleNearPlane(
			const Vector3& p,
			const Vector3& q,
			const Vector3& r,
			float nearPlane);

	private:

		//로컬 정점 -> 월드 좌표-> 카메라 좌표 계산
		void TransformVertices(
			const Mesh& mesh,
			const Matrix4x4& model,
			const Matrix4x4& view,
			std::vector<Vector3>& worldPositions,
			std::vector<Vector3>& cameraPositions
		) const;

		// 이 삼각형이 어느 방향을 바라보는지 구한다.
		Vector3 CalculateWorldNormal(
			const Vector3& point0,
			const Vector3& point1,
			const Vector3& point2
		) const;


		// 삼각형을 렌더링 하는 함수.
		void RenderTriangles(
			const Mesh& mesh,
			const std::vector<Vector3>& worldPositions,
			const std::vector<Vector3>& cameraPositions,
			const RenderView& renderView,
			const Vector3& lightDirection,
			const std::vector<char>& shadeCharacters
		) const;

		// 깊이에 따라 크게 또는 작게 보이도록 정점좌표를 계산
		void ProjectVertices(
			const std::vector<Vector3>& cameraPositions,
			const RenderView& renderView,
			std::vector<Vector2>& screenPositions,
			std::vector<float>& depths
		) const;

		//이면을 어떤 문자로 그릴지 계산하는 함수
		char CalculateShadeCharacter(
			const Vector3& worldNormal,
			const Vector3& lightDirection,
			const std::vector<char>& shadeCharacters
		) const;


		// 삼각형 내부 픽셀의 깊이와 출력을 요청하는 함수.
		void DrawTriangle(
			const Vector2& point0,
			const Vector2& point1,
			const Vector2& point2,
			float depth0,
			float depth1,
			float depth2,
			const Vector2& screenSize,
			char shadeCharacter
		) const;
	};
}
