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

    void WallTileBuilder::Build(
        DoomMap& map,
        const Vector3& worldPosition
    )
    {
        const int layers = map.GetWallLayers();

        // 충돌체는 쌓인 벽 전체를 감싸는 하나만 만든다.
        const float halfHeight = static_cast<float>(layers) * 0.5f;

        // 바닥은 유지하고, 높아진 벽의 중심을 위로 옮긴다.
        Vector3 wallCenter = worldPosition;
        wallCenter.y -= halfHeight - 0.5f;

        const auto scene = map.GetOwner();
        const auto wall = scene->Instantiate<GameObject>(wallCenter);

        wall->AddComponent<BoxCollider3DComponent>(
            Vector3(0.5f, halfHeight, 0.5f)
        );

        Mesh& mapMesh = map.GetMapMesh();

        // 각 층에서 같은 크기의 큐브와 UV를 재사용한다
        const Mesh wallMesh = MeshFactory::CreateCube(1.0f);

        // 맵의 Transform이 적용되므로 정점은 맵 기준 위치로 저장한다
        const Vector3 localPosition =
            worldPosition - map.GetWorldPosition();

        // 옆 칸에도 벽이 있는지 확인하기 위한 맵 칸 번호
        const int x = static_cast<int>(std::round(localPosition.x));
        const int z = static_cast<int>(std::round(localPosition.z));

        for (int layer = 0; layer < layers; ++layer)
        {
            // 이번 층의 정점이 전체 맵 메시에서 시작하는 번호
            const std::size_t vertexOffset = mapMesh.vertices.size();

            // 내 엔진은 -Y가 위쪽이므로 층마다 Y를 1씩 줄인다
            Vector3 layerPosition = localPosition;
            layerPosition.y -= static_cast<float>(layer);

            // 위치만 옮기고 UV는 유지해서 무늬가 늘어나지 않게 한다
            for (Vertex vertex : wallMesh.vertices)
            {
                vertex.position += layerPosition;
                mapMesh.vertices.push_back(vertex);
            }

            for (std::size_t idx = 0; idx < wallMesh.triangles.size(); ++idx)
            {
                const Triangle& triangle = wallMesh.triangles[idx];

                // 큐브의 한 면은 삼각형 두 개로 구성된다.
                const std::size_t faceIndex = idx / 2;

                // 앞뒤좌우 면은 옆 칸이 벽이면 가려지므로 제외한다.
                if (faceIndex < 4)
                {
                    const int neighborX = x + dirX[faceIndex];
                    const int neighborZ = z + dirZ[faceIndex];

                    if (map.IsWallTile(neighborX, neighborZ))
                        continue;
                }

                // 윗면- 가장 위층에만 남긴다.
                if (faceIndex == 4 && layer < layers - 1)
                    continue;

                // 아랫면- 가장 아래층에만 남긴다.
                if (faceIndex == 5 && layer > 0)
                    continue;

                // 큐브 안의 정점 번호를 전체 맵의 정점 번호로 바꾼다.
                mapMesh.triangles.push_back({
                    triangle.index0 + vertexOffset,
                    triangle.index1 + vertexOffset,
                    triangle.index2 + vertexOffset
                    });
            }
        }
    }
}


