#include "ModelMeshConverter.h"

namespace Hiwoong
{
    Mesh ConvertToRenderMesh(const ModelMesh& modelMesh)
    {
        Mesh mesh;

        mesh.vertices.reserve(modelMesh.vertices.size());
        mesh.triangles.reserve(modelMesh.triangles.size());

        for (const ModelVertex& vertex : modelMesh.vertices)
        {
            mesh.vertices.push_back(
                Vertex{ Vector3(vertex.x, vertex.y, vertex.z) }
            );
        }

        for (const ModelTriangle& triangle : modelMesh.triangles)
        {
            mesh.triangles.push_back(
                Triangle{
                    triangle.index0,
                    triangle.index1,
                    triangle.index2
                }
            );
        }

        return mesh;
    }
}