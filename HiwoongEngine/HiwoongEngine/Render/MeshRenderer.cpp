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



		//월드 좌표 보관
		std::vector<Vector3> worldPositions;
		worldPositions.reserve(mesh.vertices.size());

		//광원방향 (Test)
		const Vector3 lightDirection =
			Vector3(-1.0f, -1.0f, -1.0f).Normalized();

		//카메라 좌표 저장소.
		std::vector<Vector3> cameraPositions;
		cameraPositions.reserve(mesh.vertices.size());

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
			
			// 1. 로컬 → 월드
			const Vector4 worldPosition4 =
				model * localPosition;

			// 2. 월드 → 카메라
			const Vector4 cameraPosition4 =
				renderView.view * worldPosition4;



			//각 정점의 cameraPosition.z를 nearPlane과 비교해서 자를지 말지를 결정해야함.
			cameraPositions.emplace_back(
				cameraPosition4.x,
				cameraPosition4.y,
				cameraPosition4.z
			);

			const Vector3 worldPosition( //Vector3로 변환
				worldPosition4.x,
				worldPosition4.y,
				worldPosition4.z
			);
			worldPositions.emplace_back(worldPosition);
		}

		//Triangle이 쓰는 정점 세개의 화면 좌표 찾기.
		for (const Triangle& triangle : mesh.triangles)
		{
			const Vector3& cameraPoint0 =
				cameraPositions[triangle.index0];

			const Vector3& cameraPoint1 =
				cameraPositions[triangle.index1];

			const Vector3& cameraPoint2 =
				cameraPositions[triangle.index2];

			const std::vector<Vector3> clippedPositions =
				ClipTriangleNearPlane(
					cameraPoint0,
					cameraPoint1,
					cameraPoint2,
					renderView.nearPlane
				);

			if (clippedPositions.empty())
			{
				continue;
			}


			std::vector<Vector2> clippedScreenPositions;
			std::vector<float> clippedDepths;

			for (const Vector3& clippedPosition : clippedPositions)
			{
				// 카메라 좌표 Vector3 → 위치 Vector4
				Vector4 cameraPosition(
					clippedPosition.x,
					clippedPosition.y,
					clippedPosition.z,
					1.0f
				);

				// 카메라 좌표 → Clip 좌표
				Vector4 clipPosition =
					renderView.projection * cameraPosition;

				Vector3 ndcPosition =
					clipPosition.PerspectiveDivide();

				// NDC 좌표 → 콘솔 화면 좌표
				Vector2 screenPosition =
					SoftwareRasterizer::NdcToScreen(
						ndcPosition,
						renderView.screenSize.x,
						renderView.screenSize.y
					);

				clippedScreenPositions.emplace_back(screenPosition);
				clippedDepths.emplace_back(ndcPosition.z);
			}

			for (std::size_t i = 1;
				i + 1 < clippedScreenPositions.size();
				++i)
			{
				const Vector2& point0 = clippedScreenPositions[0];
				const Vector2& point1 = clippedScreenPositions[i];
				const Vector2& point2 = clippedScreenPositions[i + 1];

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

				const float brightness = std::clamp(lightAmount, 0.0f, 1.0f);


				//카메라 반대쪽은 내부를 채울 필요가 없으니 렌더링 영역에서 제외
				if (SoftwareRasterizer::IsBackFace(point0, point1, point2)) continue;

				//살아남은 삼각형 내부의 화면 칸을 구하기.
				const std::vector<Vector2> pixels =
					SoftwareRasterizer::RasterizeTriangle(point0, point1, point2,
						renderView.screenSize.x,renderView.screenSize.y);


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
						weights.x * clippedDepths[0] +
						weights.y * clippedDepths[i] +
						weights.z * clippedDepths[i + 1];


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

	std::vector<Vector3> MeshRenderer::ClipTriangleNearPlane(const Vector3& p, const Vector3& q, const Vector3& r, float nearPlane)
	{
		std::vector<Vector3> vertices = { p, q, r };
		std::vector<bool> insides(3, false);

		std::vector<Vector3> positions;

		int insideCount = 0;

		//어떤 정점이 밖에 있나 확인.
		for (int i = 0; i < vertices.size(); ++i)
		{
			if (vertices[i].z >= nearPlane)
			{
				insideCount++;
				insides[i] = true;
			}
		}


		switch (insideCount)
		{
		case 0: return {};
		case 1:
		{
			int findidx = -1;
			for (int i = 0; i < insides.size(); ++i)
			{
				if (insides[i] == true)
				{
					findidx = i;
					break;
				}
			}
			//찾은 정점에서 이전과 이후의 정점을 찾은 다음.
			int nextIdx = (findidx + 1) % 3;
			int preIdx = (findidx + 2) % 3;
			//찾은 정점과 바깥쪽 정점 사이의 정점 두개를 구한다.
			Vector3 j1 = Vector3::IntersectNearPlane(vertices[findidx], vertices[nextIdx], nearPlane);
			Vector3 j2 = Vector3::IntersectNearPlane(vertices[findidx], vertices[preIdx], nearPlane);
			return{ vertices[findidx],j1,j2 };
		}
		case 2:
		{
			int outidx = -1;//나간정점
			for (int i = 0; i < insides.size(); ++i)
			{
				if (insides[i] == false)
				{
					outidx = i;
					break;
				}
			}
			//나간 정점에서 이전과 이후의 정점을 찾은 다음.
			int nextIdx = (outidx + 1) % 3;
			int preIdx = (outidx + 2) % 3;
			//나간과 안쪽 정점 사이의 정점 두개를 구한다.
			Vector3 j1 = Vector3::IntersectNearPlane(vertices[outidx], vertices[nextIdx], nearPlane);
			Vector3 j2 = Vector3::IntersectNearPlane(vertices[outidx], vertices[preIdx], nearPlane);
			return{ vertices[nextIdx],vertices[preIdx],j2,j1 };
		}

		case 3: return { p,q,r };
		default: break;
		}

		return {};
	}

}
