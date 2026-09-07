#include "WallTileBuilder.h"
#include "Map/DoomMap.h"
#include "Render/MeshFactory.h"
#include "Render/Vertex.h"
namespace Hiwoong
{
	void WallTileBuilder::Build(DoomMap& map, const Vector3& worldPosition)
	{
		//벽의 메쉬 만들기
		Mesh& mapMesh = map.GetMapMesh();

		const Mesh wallMesh = MeshFactory::CreateCube(1.0f);
		//벽의 기준값을 정하는 오프셋 예를들어 벽이 하나 생성됐다면 그 다음 위치가 될 인덱스
		const std::size_t vertexOffset = mapMesh.vertices.size();

		//원본 큐브의 정점을 맵에서 읽은 #좌표로 옮겨 mapMesh에 넣는다.
		for (const Vertex& vertex : wallMesh.vertices)
		{
			const Vector3 movedPosition = vertex.position + worldPosition;

			mapMesh.vertices.emplace_back(Vertex{ movedPosition });
		}

		//삼각형을 추가한다. 단 자기 위치에 맞게 추가해야하므로 offset를 더한다.
		for (const Triangle& triangle : wallMesh.triangles)
		{
			mapMesh.triangles.emplace_back(
				Triangle{
					triangle.index0 + vertexOffset,
					triangle.index1 + vertexOffset,
					triangle.index2 + vertexOffset
				}
			);
		}

	}
}


