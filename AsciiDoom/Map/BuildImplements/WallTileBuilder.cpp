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
        Mesh& mapMesh = map.GetMapMesh();
        const Mesh wallMesh = MeshFactory::CreateCube(1.0f);

        const std::size_t vertexOffset = mapMesh.vertices.size();

        // 충돌용 오브젝트는 기존처럼 월드 위치에 생성
        const std::shared_ptr<Scene> scene = map.GetOwner();
        const std::shared_ptr<GameObject> wall =
            scene->Instantiate<GameObject>(worldPosition);

        wall->AddComponent<BoxCollider3DComponent>(
            Vector3(0.5f, 0.5f, 0.5f)
        );

        // 9.18일 변경 부분: 메시 정점에는 맵 기준 위치를 더한다.
        const Vector3 localPosition =
            worldPosition - map.GetWorldPosition();

        for (const Vertex& vertex : wallMesh.vertices)
        {
            const Vector3 movedPosition = vertex.position + localPosition;
            mapMesh.vertices.emplace_back(Vertex{ movedPosition });
        }

        // 아래는 기존에 있던 이웃 벽 검사와 삼각형 추가 처리
        const int x = static_cast<int>(std::round(localPosition.x));
        const int z = static_cast<int>(std::round(localPosition.z));

        for (std::size_t idx = 0; idx < wallMesh.triangles.size(); ++idx)
        {
            const Triangle& triangle = wallMesh.triangles[idx];
            const std::size_t faceIndex = idx / 2;

            if (faceIndex < 4)
            {
                const int neighborX = x + dirX[faceIndex];
                const int neighborZ = z + dirZ[faceIndex];

                if (map.IsWallTile(neighborX, neighborZ))
                    continue;
            }

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


