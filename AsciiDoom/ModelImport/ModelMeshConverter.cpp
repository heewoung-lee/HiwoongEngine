#include "ModelMeshConverter.h"

namespace Hiwoong
{
	Mesh ConvertToRenderMesh(
		const ModelMesh& modelMesh,
		std::size_t materialIndex
	)
	{
		Mesh mesh;

		for (const ModelVertex& vertex : modelMesh.vertices)
		{
			mesh.vertices.push_back(Vertex{
				Vector3(vertex.x, vertex.y, vertex.z),
				vertex.u,
				vertex.v
				});
		}

		for (const ModelTriangle& triangle : modelMesh.triangles)
		{
			if (triangle.materialIndex != materialIndex)
				continue;

			mesh.triangles.push_back(Triangle{
				triangle.index0,
				triangle.index1,
				triangle.index2
				});
		}

		return mesh;
	}
}