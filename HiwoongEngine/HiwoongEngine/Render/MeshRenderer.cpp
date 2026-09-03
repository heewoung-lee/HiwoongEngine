#include "MeshRenderer.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector4.h"
#include "Render/SoftwareRasterizer.h"
#include "Render/Renderer.h"
#include "Math/Color.h"
#include <algorithm>
#include <vector>
namespace Hiwoong
{

	void MeshRenderer::Render(
		const Mesh& mesh, 
		const Matrix4x4& model,
		const RenderView& renderView) const
	{

		//임시용임 없앨것.(Test)
		std::vector<char> rightRender = { '.',':','+','#','@' };




		//모델 x 뷰 x 프로젝션
		//현재 엔진이 열벡터 계산형식으로 되어있어서 뒤집어서 계산해야한다.
		const Matrix4x4 mvp =
			renderView.projection * renderView.view * model;

		//공간 확보
		std::vector<Vector3> ndcPositions;
			ndcPositions.reserve(mesh.vertices.size());


		//월드 좌표 보관
		std::vector<Vector3> worldPositions;
		worldPositions.reserve(mesh.vertices.size());

		//광원방향 (Test)
		const Vector3 lightDirection =
			Vector3(-1.0f, -1.0f, -1.0f).Normalized();



		for (const Vertex& vertex : mesh.vertices)
		{
			//위치 벡터이므로 w = 1표시
			const Vector4 localPosition
			(
				vertex.position.x,
				vertex.position.y,
				vertex.position.z,
				1.0f
			);
			
			// 각 로컬 정점에 Model → View → Projection을 적용한 Clip 좌표가 저장
			const Vector4 clipPosition = mvp * localPosition;

			//w로 z거리에 대한 원근을 각 정점마다 계산 원근 나누기
			Vector3 ndcPosition = clipPosition.PerspectiveDivide();

			ndcPositions.emplace_back(ndcPosition);

			const Vector4 worldPosition4 = model * localPosition;
			const Vector3 worldPosition( //Vector3로 변환
				worldPosition4.x,
				worldPosition4.y,
				worldPosition4.z
			);
			worldPositions.emplace_back(worldPosition);
		}




		//화면 좌표
		std::vector<Vector2> screenPositions;
		screenPositions.reserve(mesh.vertices.size());


		//깊이
		std::vector<float> depths;
		depths.reserve(mesh.vertices.size());

		for (const Vector3& ndcPos : ndcPositions)
		{
			Vector2 screenPos = SoftwareRasterizer::NdcToScreen(
				ndcPos,
				renderView.screenSize.x,
				renderView.screenSize.y);

			screenPositions.emplace_back(screenPos);
			depths.emplace_back(ndcPos.z);
		}//같은 인덱스를 사용해, O(1)로 깊이를 바로 가져올 수 있게 매칭.

		//Triangle이 쓰는 정점 세개의 화면 좌표 찾기.
		for (const Triangle& triangle : mesh.triangles)
		{
			const Vector2& point0 = screenPositions[triangle.index0];
			const Vector2& point1 = screenPositions[triangle.index1];
			const Vector2& point2 = screenPositions[triangle.index2];


			const Vector3& worldPoint0 = worldPositions[triangle.index0];
			const Vector3& worldPoint1 = worldPositions[triangle.index1];
			const Vector3& worldPoint2 = worldPositions[triangle.index2];


			//두변을 외적해서 월드 법선을 구하기 위함,
			//월드 법선으로 음영을 구한다.
			Vector3 edge01 = worldPoint1 - worldPoint0;
			Vector3 edge02 = worldPoint2 - worldPoint0;

			//외적한뒤 정규화 길이를 1로 만들어 0~1까지의 조명계산용
			const Vector3 worldNormal =
				Vector3::Cross(edge01, edge02).Normalized();


			//삼각형이 바라보고 있는 방향과 광원이 있는 월드방향을 내적
			const float lightAmount = Vector3::Dot(lightDirection, worldNormal);

			const float brightness =std::clamp(lightAmount, 0.0f, 1.0f);


			//카메라 반대쪽은 내부를 채울 필요가 없으니 렌더링 영역에서 제외
			if (SoftwareRasterizer::IsBackFace(point0, point1, point2)) continue;

			//살아남은 삼각형 내부의 화면 칸을 구하기.
			const std::vector<Vector2> pixels =
				SoftwareRasterizer::RasterizeTriangle(point0, point1, point2);


			const std::size_t rightRenderIdx =
				static_cast<std::size_t>(brightness * (rightRender.size() - 1));


			//바리센트릭 가중치를 사용해,
			//각 픽셀들의 깊이값을 계산.
			//우리가 가진 정보는 세 정점의 깊이 밖에 모르니.
			//픽셀이 세 정점으로 부터 얼마나 떨어져있는지에 대한 비율을 계산하면,
			//픽셀의 깊이를 계산할 수 있음.
			for (const Vector2& pixel : pixels)
			{
				const Vector3 weights =
					SoftwareRasterizer::CalculateBarycentric(
						pixel,
						point0,
						point1,
						point2);

				//픽셀 깊이.
				const float depth =
					weights.x * depths[triangle.index0] +
					weights.y * depths[triangle.index1] +
					weights.z * depths[triangle.index2];


				//렌더러에게 이거 그려주세요 라고 제출.
				Renderer::Get().SubmitPoint3D
				(
					pixel, //픽셀위치
					depth, //픽셀깊이
					rightRender[rightRenderIdx], //렌더링 문자 
					Color::White, // 렌러딩 색상
					0 // 정렬 순서
				);
			}
		}
		
		



	}

}
