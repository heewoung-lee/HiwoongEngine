#include "WallTileBuilder.h"
#include "Map/DoomMap.h"
#include "Render/MeshFactory.h"
#include "Render/Vertex.h"
#include "Scene/Scene.h"
#include "Component/BoxCollider3DComponent.h"
#include <cmath>
#include <vector>
namespace Hiwoong
{
	const std::vector<int> dirX = { 0,0,-1,1 };
	const std::vector<int> dirZ = { -1,1,0,0 };

	void WallTileBuilder::Build(DoomMap& map, const Vector3& worldPosition)
	{
		//벽의 메쉬 만들기
		Mesh& mapMesh = map.GetMapMesh();

		const Mesh wallMesh = MeshFactory::CreateCube(1.0f);
		//벽의 기준값을 정하는 오프셋 예를들어 벽이 하나 생성됐다면 그 다음 위치가 될 인덱스


		const std::size_t vertexOffset = mapMesh.vertices.size();

		const std::shared_ptr<Scene> scene = map.GetOwner();
		const std::shared_ptr<GameObject> wall = scene->Instantiate<GameObject>(worldPosition);

		wall->AddComponent<BoxCollider3DComponent>(Vector3(0.5f, 0.5f, 0.5f));

		//원본 큐브의 정점을 맵에서 읽은 #좌표로 옮겨 mapMesh에 넣는다.
		for (const Vertex& vertex : wallMesh.vertices)
		{
			const Vector3 movedPosition = vertex.position + worldPosition;

			mapMesh.vertices.emplace_back(Vertex{ movedPosition });
		}

		const Vector3 localPosition =
			worldPosition - map.GetWorldPosition();

		const int x = static_cast<int>(std::round(localPosition.x));
		const int z = static_cast<int>(std::round(localPosition.z));


		//삼각형을 추가한다. 단 자기 위치에 맞게 추가해야하므로 offset를 더한다.
		for (std::size_t idx = 0; idx < wallMesh.triangles.size();++idx)
		{
			const Triangle& triangle = wallMesh.triangles[idx];

			
			//삼각형 두개가 한 면이므로 면의인덱스 계산은 /2로 함
			//0:앞, 1: 뒤 , 2: 왼쪽 3: 오른쪽 4: 위 , 5: 아래
			//하는 이유는 만약 벽이 이어져 있다면, 굳이 안쪽을 그릴 필요없음.
			//또한 버그도 있었는데, 큐브 뒷면과 큐브 앞면이 닿아 버리니
			//경계선이 생기는 문제도 발생.
			const std::size_t faceIndex = idx / 2;
			
			//위 아래는 검사할 필요 없음
			//아웃된 타일에서 벽이 있는지 확인
			if (faceIndex < 4)
			{
				const int neighborX = x + dirX[faceIndex];
				const int neighborZ = z + dirZ[faceIndex];

				if (map.IsWallTile(neighborX, neighborZ))
					continue;
			}

			//이웃된 벽이 아니면 현재 삼각형을 추가한다.
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


