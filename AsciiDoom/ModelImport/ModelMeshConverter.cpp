#include "ModelMeshConverter.h"

namespace Hiwoong
{
    Mesh ConvertToRenderMesh(
        const ModelMesh& modelMesh,
        std::size_t materialIndex
    )
    {
        Mesh mesh;

        for (const ModelTriangle& triangle : modelMesh.triangles)
        {
            if (triangle.materialIndex != materialIndex)
                continue;

            const std::size_t firstVertexIndex = mesh.vertices.size();

            const std::size_t sourceIndices[3] = {
                triangle.index0,
                triangle.index1,
                triangle.index2
            };

            for (std::size_t sourceIndex : sourceIndices)
            {
                const ModelVertex& vertex = modelMesh.vertices[sourceIndex];

                mesh.vertices.push_back(Vertex{
                    Vector3(vertex.x, vertex.y, vertex.z),
                    vertex.u,
                    vertex.v
                    });
            }

            // 방금 추가한 정점 세 개를 연결한다.
            mesh.triangles.push_back(Triangle{
                firstVertexIndex,
                firstVertexIndex + 1,
                firstVertexIndex + 2
                });
        }

        return mesh;
    }
}