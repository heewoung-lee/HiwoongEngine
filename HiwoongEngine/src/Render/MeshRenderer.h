#pragma once
#include "Core/Core.h"
#include "Render/Mesh.h"
#include "Math/Matrix4x4.h"
#include "Render/RenderView.h"
#include "Math/Color.h"
#include "Render/IRenderable3D.h"

namespace Hiwoong
{
	class Hiwoong_API MeshRenderer
	{


	public : 
		//9.18일 추가. 
		void Render(
			const IRenderable3D& renderable,
			const RenderView& renderView
		) const;


		
	private:
		// 9.19일 추가
		// 근평면 클리핑 과정에서 카메라 좌표와 UV를 함께 전달하는 정점.
		// 교차 지점이 생기면 위치와 같은 비율로 UV도 보간한다.
		// 잘려서 생긴 정점에는 uv가 없기 때문에
		// 픽셀 셰이딩을 할 때 문자를 못가져오는 문제가 생길 수 있기에 정점 데이터를 추가.
		struct RenderVertex
		{
			Vector3 cameraPosition;
			float u;
			float v;
		};

		//삼각형 자르기
		static std::vector<RenderVertex> ClipTriangleNearPlane(
			const RenderVertex& p,
			const RenderVertex& q,
			const RenderVertex& r,
			float nearPlane);

		//위치와 uv를 함께 계산하기 위한 함수
		static RenderVertex IntersectNearPlane(
			const RenderVertex& start,
			const RenderVertex& end,
			float nearPlane
		);

	private:
		//로컬 정점 -> 월드 좌표-> 카메라 좌표 계산
		void TransformVertices(
			const Mesh& mesh,
			const Matrix4x4& model,
			const Matrix4x4& view,
			std::vector<Vector3>& worldPositions,
			std::vector<RenderVertex>& renderVertices
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
			const std::vector<RenderVertex>& renderVertices,
			const RenderView& renderView,
			const Vector3& lightDirection,
			const IRenderable3D& renderable,
			Color color
		) const;

		// 깊이에 따라 크게 또는 작게 보이도록 정점좌표를 계산
		void ProjectVertices(
			const std::vector<RenderVertex>& renderVertices,
			const RenderView& renderView,
			std::vector<Vector2>& screenPositions,
			std::vector<float>& depths
		) const;

		// 삼각형 내부 픽셀의 깊이와 출력을 요청하는 함수.
		void DrawTriangle(
			const Vector2& point0,
			const Vector2& point1,
			const Vector2& point2,
			float depth0,
			float depth1,
			float depth2,
			const RenderVertex& renderVertex0,
			const RenderVertex& renderVertex1,
			const RenderVertex& renderVertex2,
			const RenderView& renderView,
			const Vector3& worldNormal,
			float baseBrightness,
			const IRenderable3D& renderable,
			Color color
		) const;

		//방향광 밝기 계산 함수
		float CalculateDirectionalBrightness(
			const Vector3& worldNormal,
			const Vector3& lightDirection
		) const;

		/*worldPosition: 빛을 받는 표면 지점의 월드 위치
		worldNormal : 그 표면이 향하는 방향
		light : 손전등의 위치·방향·거리·각도·세기*/
		float CalculateSpotBrightness(
			const Vector3& worldPosition,
			const Vector3& worldNormal,
			const SpotLight& light
		) const;


	};
}
