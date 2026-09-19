#include "MeshRenderer.h"
#include "Math/Matrix4x4.h"
#include "Math/Vector4.h"
#include "Render/SoftwareRasterizer.h"
#include "Render/Renderer.h"
#include "Math/Color.h"
#include "Math/MathConstants.h"
#include <cmath>
#include <algorithm>
#include <vector>
namespace Hiwoong
{
	void MeshRenderer::Render(
		const IRenderable3D& renderable,
		const RenderView& renderView) const
	{
		const Mesh& mesh = renderable.GetMesh();
		const Matrix4x4 model = renderable.GetModelMatrix(renderView);
		const Color color = renderable.GetRenderColor();

		const Vector3 lightDirection =
			Vector3(-1.0f, -1.0f, -1.0f).Normalized();


		std::vector<Vector3> worldPositions;
		worldPositions.reserve(mesh.vertices.size());

		std::vector<RenderVertex> renderVertices;

		renderVertices.reserve(mesh.vertices.size());


		TransformVertices(
			mesh, model, renderView.view,
			worldPositions, renderVertices
		);

		RenderTriangles(
			mesh, worldPositions, renderVertices,
			renderView, lightDirection, renderable, color
		);
	}

	MeshRenderer::RenderVertex MeshRenderer::IntersectNearPlane(const RenderVertex& start, const RenderVertex& end, float nearPlane)
	{
		//비율
		const float t = (nearPlane - start.cameraPosition.z) / (end.cameraPosition.z - start.cameraPosition.z);
		
		//근평면과 만나는 새 정점 위치.
		const Vector3 cameraPos = start.cameraPosition + (end.cameraPosition - start.cameraPosition) * t;

		const float u = start.u + (end.u - start.u) * t;
		const float v = start.v + (end.v - start.v) * t;
		
		//카메라 포지션과 uv 좌표 뱉기
		//기존에는 자른 쪽의 포지션만 배출했는데  uv를 같이 관리해야해서 묶음으로 전달
		
		return { cameraPos, u, v };
	}


	std::vector<MeshRenderer::RenderVertex>
		MeshRenderer::ClipTriangleNearPlane(
			const RenderVertex& p,
			const RenderVertex& q,
			const RenderVertex& r,
			float nearPlane)
	{
		std::vector<RenderVertex> vertices = { p, q, r };
		std::vector<bool> insides(3, false);

		int insideCount = 0;

		//어떤 정점이 밖에 있나 확인.
		for (int i = 0; i < vertices.size(); ++i)
		{
			if (vertices[i].cameraPosition.z >= nearPlane)
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
			RenderVertex j1 = IntersectNearPlane(vertices[findidx], vertices[nextIdx], nearPlane);
			RenderVertex j2 = IntersectNearPlane(vertices[findidx], vertices[preIdx], nearPlane);
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
			RenderVertex j1 = IntersectNearPlane(vertices[outidx], vertices[nextIdx], nearPlane);
			RenderVertex j2 = IntersectNearPlane(vertices[outidx], vertices[preIdx], nearPlane);
			return{ vertices[nextIdx],vertices[preIdx],j2,j1 };
		}

		case 3: return { p,q,r };
		default: break;
		}

		return {};
	}


	void MeshRenderer::TransformVertices(
		const Mesh& mesh,
		const Matrix4x4& model,
		const Matrix4x4& view,
		std::vector<Vector3>& worldPositions,
		std::vector<RenderVertex>& renderVertices
	) const
	{
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
				view * worldPosition4;

			//각 정점의 cameraPosition.z를 nearPlane과 비교해서 자를지 말지를 결정해야함.
			const Vector3 cameraPosition
			(
				cameraPosition4.x,
				cameraPosition4.y,
				cameraPosition4.z
			);
			
			renderVertices.push_back({
				cameraPosition,
				vertex.u,
				vertex.v
				});

			const Vector3 worldPosition( //Vector3로 변환
				worldPosition4.x,
				worldPosition4.y,
				worldPosition4.z
			);
			worldPositions.emplace_back(worldPosition);
		}
	}

	Vector3 MeshRenderer::CalculateWorldNormal(const Vector3& point0, const Vector3& point1, const Vector3& point2) const
	{
		//두변을 외적해서 월드 법선을 구하기 위함,
				//월드 법선으로 음영을 구한다.
		Vector3 edge01 = point1 - point0;
		Vector3 edge02 = point2 - point0;

		//외적한뒤 정규화 길이를 1로 만들어 0~1까지의 조명계산용
		const Vector3 worldNormal =
			Vector3::Cross(edge01, edge02).Normalized();


		return worldNormal;
	}


	void MeshRenderer::RenderTriangles(
		const Mesh& mesh,
		const std::vector<Vector3>& worldPositions,
		const std::vector<RenderVertex>& renderVertices,
		const RenderView& renderView,
		const Vector3& lightDirection,
		const IRenderable3D& renderable,
		Color color) const
	{
		//Triangle이 쓰는 정점 세개의 화면 좌표 찾기.
		for (const Triangle& triangle : mesh.triangles)
		{
			const RenderVertex& renderVertex0 =
				renderVertices[triangle.index0];

			const RenderVertex& renderVertex1 =
				renderVertices[triangle.index1];

			const RenderVertex& renderVertex2 =
				renderVertices[triangle.index2];

			const std::vector<RenderVertex> clippedVertices =
				ClipTriangleNearPlane(
					renderVertex0,
					renderVertex1,
					renderVertex2,
					renderView.nearPlane
				);

			if (clippedVertices.empty())
			{
				continue;
			}


			std::vector<Vector2> clippedScreenPositions;
			std::vector<float> clippedDepths;

			ProjectVertices(clippedVertices, renderView, clippedScreenPositions, clippedDepths);

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


				//외적한뒤 정규화 길이를 1로 만들어 0~1까지의 조명계산용
				const Vector3 worldNormal = CalculateWorldNormal(worldPoint0, worldPoint1, worldPoint2);


				//현재 빛이 얼마만큼 묻는 지를 계산하고, 빛을 받은 양에 따라 문자 결정
				const float ambientBrightness = renderView.ambientBrightness;
				const float directionalIntensity = renderView.directionalIntensity;

				const float brightness =
					ambientBrightness +
					directionalIntensity *
					CalculateDirectionalBrightness(worldNormal, lightDirection);




				//카메라 반대쪽은 내부를 채울 필요가 없으니 렌더링 영역에서 제외
				if (SoftwareRasterizer::IsBackFace(point0, point1, point2)) continue;
				DrawTriangle(
					point0, point1, point2,
					clippedDepths[0], clippedDepths[i], clippedDepths[i + 1],
					clippedVertices[0],
					clippedVertices[i],
					clippedVertices[i + 1],
					renderView,
					worldNormal,
					brightness,
					renderable,
					color
				);
			}
		}


	}

	void MeshRenderer::ProjectVertices(
		const std::vector<RenderVertex>& renderVertices,
		const RenderView& renderView,
		std::vector<Vector2>& screenPositions,
		std::vector<float>& depths) const
	{

		for (const RenderVertex& renderVertex : renderVertices)
		{
			const Vector3& clippedPosition =
				renderVertex.cameraPosition;

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



			screenPositions.emplace_back(screenPosition);
			depths.emplace_back(ndcPosition.z);
		}
	}
	void MeshRenderer::DrawTriangle(
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
		Color color) const
	{

		//살아남은 삼각형 내부의 화면 칸을 구하기.
		const std::vector<Vector2> pixels =
			SoftwareRasterizer::RasterizeTriangle(point0, point1, point2,
				renderView.screenSize.x, renderView.screenSize.y);

		//화면에서는 가까운 부분이 크게,
		// 먼 부분이 작게 보이므로 공간 좌표를 단순히 섞으면 위치가 틀어짐
		// 이 깊이의 역수 1/z를 이용해 그 차이를 보정
		const float inverseZ0 = 1.0f / renderVertex0.cameraPosition.z;
		const float inverseZ1 = 1.0f / renderVertex1.cameraPosition.z;
		const float inverseZ2 = 1.0f / renderVertex2.cameraPosition.z;

		//바리센트릭 가중치를 사용해,
		//각 픽셀들의 깊이값을 계산.
		//우리가 가진 정보는 세 정점의 깊이 밖에 모르니.
		//픽셀이 세 정점으로 부터 얼마나 떨어져있는지에 대한 비율을 계산하면,
		//픽셀의 깊이를 계산할 수 있음.
		for (const Vector2& pixel : pixels)
		{
			//화면 비중을 구하는 작업. 
			const Vector3 weights =
				SoftwareRasterizer::CalculateBarycentric(
					pixel,
					point0,
					point1,
					point2);

			//화면 비중을 깊이로 나눈 값들으 ㅣ 합.
			const float interpolatedInverseZ =
				weights.x * inverseZ0 +
				weights.y * inverseZ1 +
				weights.z * inverseZ2;

			//원근 보정해서 계산한 uv
			const float u = 
				(
					renderVertex0.u * weights.x * inverseZ0 +
					renderVertex1.u * weights.y * inverseZ1 +
					renderVertex2.u * weights.z * inverseZ2 
				) / interpolatedInverseZ;


			const float v =
				(
					renderVertex0.v * weights.x * inverseZ0 +
					renderVertex1.v * weights.y * inverseZ1 +
					renderVertex2.v * weights.z * inverseZ2
					) / interpolatedInverseZ;


			const Vector3 cameraPosition =
				(
					renderVertex0.cameraPosition * (weights.x * inverseZ0) +
					renderVertex1.cameraPosition * (weights.y * inverseZ1) +
					renderVertex2.cameraPosition * (weights.z * inverseZ2)
					) * (1.0f / interpolatedInverseZ);

			//픽셀의 카메라 좌표를 월드 좌표로 변환.
			const Vector4 worldPosition4 =
				renderView.cameraToWorld *
				Vector4(
					cameraPosition.x,
					cameraPosition.y,
					cameraPosition.z,
					1.0f
				);

			const Vector3 worldPosition(
				worldPosition4.x,
				worldPosition4.y,
				worldPosition4.z
			);

			const float spotBrightness = CalculateSpotBrightness(
				worldPosition,
				worldNormal,
				renderView.spotLight
			);

			//9.19일 변경 Irenderable을 통해.
			//3D렌러링이 필요한 정보를 제공할 수 있게.
			//데이터 통로를 한방에 모아서 설계함.

			char character;

			//이 위치에 픽셀정보가 있는지 확인.
			if (renderable.TryGetCharactor(
				u,
				v,
				baseBrightness + spotBrightness,
				character) == false)
			{
				continue;
			}

			//픽셀 깊이.
			const float depth =
				weights.x * depth0 +
				weights.y * depth1 +
				weights.z * depth2;


			//렌더러에게 이거 그려주세요 라고 제출.
			Renderer::Get().SubmitPoint3D
			(
				pixel, //픽셀위치
				depth, //픽셀깊이
				character, //렌더링 문자 
				color, // 렌러딩 색상
				0 // 정렬 순서
			);
		}
	}

	float MeshRenderer::CalculateDirectionalBrightness(const Vector3& worldNormal, const Vector3& lightDirection) const
	{
		const float lightAmount = Vector3::Dot(lightDirection, worldNormal);
		return std::clamp(lightAmount, 0.0f, 1.0f);
	}

	float MeshRenderer::CalculateSpotBrightness(
		const Vector3& worldPosition,
		const Vector3& worldNormal,
		const SpotLight& light
	) const
	{
		//표면 위치 - 손전등 위치
		const Vector3 lightToPoint = worldPosition - light.position;
		const float distance = lightToPoint.Length();
		
		if (light.range <= 0.0f ||
			distance <= 0.0f ||
			distance >= light.range)
		{
			return 0.0f;
		}

		const Vector3 directionToPoint = lightToPoint.Normalized();
		const float distanceAttenuation = 1.0f - distance / light.range;

		//바깥 각도도 코사인 값으로 바꿔 비교 
		const float cosTheta =
			Vector3::Dot(light.direction, directionToPoint);

		const float outerCos = std::cos(
			light.outerHalfAngleDegrees * MathConstants::Pi / 180.0f
		);
		//30도 이상 벗어난 지점은 밝기를 0도로 반환.
		if (cosTheta <= outerCos)
		{
			return 0.0f;
		}


		//각도에 따라 밝기가 줄어들음 30-> 20도
		const float angleAttenuation = std::clamp(
			(cosTheta - outerCos) / (1.0f - outerCos),
			0.0f,
			1.0f
		);

		// 표면이 빛을 향하는 정도
		// directionToPoint는 손전등 → 표면이므로, 
		// 반대로 뒤집어 표면 → 손전등 방향을 구함/
		// 그 방향과 면의 법선을 비교
		const Vector3 directionToLight = directionToPoint * -1.0f;

		const float surfaceBrightness = std::clamp(
			Vector3::Dot(worldNormal, directionToLight),
			0.0f,
			1.0f
		);

		return light.intensity
			* distanceAttenuation
			* angleAttenuation
			* surfaceBrightness;
	}

}
