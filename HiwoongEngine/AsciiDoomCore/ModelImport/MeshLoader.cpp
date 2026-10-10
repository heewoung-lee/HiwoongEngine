#include "MeshLoader.h"
#include "IModelSource.h"

namespace Hiwoong
{
	MeshLoadResult MeshLoader::Load(const ModelImportContext& context) const
	{
		MeshLoadResult result;

		if (context.source == nullptr)
		{
			result.errorMessage = "Model source is missing";
			return result;
		}
		
		const auto source = context.source;

        for (std::size_t meshIndex = 0;
            meshIndex < source->GetMeshCount();
            ++meshIndex)
        {
            ModelMesh mesh;
            mesh.name = source->GetMeshName(meshIndex);

            mesh.diffuseTexturePaths =
                source->GetDiffuseTexturePaths(meshIndex);


            MeshSourceMapping mapping;
            mapping.sourceMeshIndex = meshIndex;

            // 원본 정점과 대응 번호를 복사
            const std::size_t vertexCount =
                source->GetCornerCount(meshIndex);


            for (std::size_t cornerIndex = 0;
                cornerIndex < vertexCount;
                ++cornerIndex)
            {
                mesh.vertices.push_back(
                    source->GetCornerVertex(meshIndex, cornerIndex)
                );

                mapping.sourceVertexIndices.push_back(
                    source->GetCornerSourceVertexIndex(
                        meshIndex, cornerIndex
                    )
                );
            }

            // 원본 면을 삼각형 데이터로 옮김
            for (std::size_t faceIndex = 0;
                faceIndex < source->GetFaceCount(meshIndex);
                ++faceIndex)
            {
                const std::vector<ModelTriangle> triangles =
                    source->TriangulateFace(meshIndex, faceIndex);

                if (triangles.empty())
                {
                    result.errorMessage = "Face triangulation failed";
                    return result;
                }

                for (const ModelTriangle& triangle : triangles)
                {
                    if (triangle.index0 >= vertexCount ||
                        triangle.index1 >= vertexCount ||
                        triangle.index2 >= vertexCount)
                    {
                        result.errorMessage = "Face references an invalid vertex";
                        return result;
                    }

                    mesh.triangles.push_back(triangle);
                }
            }

            result.meshes.push_back(mesh);
            result.sourceMappings.push_back(mapping);
        }

        result.success = true;
        return result;

	}


}