#include "UfbxModelSource.h"
#include "../ThirdParty/ufbx.h"
#include <cstdint>
#include <cassert>

namespace Hiwoong
{
    UfbxModelSource::UfbxModelSource(ufbx_scene* scene)
        : scene(scene)
    {
    }

    UfbxModelSource::~UfbxModelSource()
    {
        ufbx_free_scene(scene);
    }
    std::size_t UfbxModelSource::GetMeshCount() const
    {
        return scene->meshes.count;
    }
    std::string UfbxModelSource::GetMeshName(
        std::size_t meshIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        return std::string(mesh->name.data, mesh->name.length);
    }
    std::size_t UfbxModelSource::GetVertexCount(
        std::size_t meshIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        return mesh->num_vertices;
    }
    ModelVertex UfbxModelSource::GetVertex(
        std::size_t meshIndex,
        std::size_t vertexIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        // 현재는 메시마다 배치된 오브젝트가 하나인 FBX를 지원한다.
        assert(mesh->instances.count == 1);

        const ufbx_node* node = mesh->instances.data[0];

        const ufbx_vec3 position = ufbx_transform_position(
            &node->geometry_to_world,
            mesh->vertices.data[vertexIndex]
        );

        return ModelVertex{
            static_cast<float>(position.x),
            static_cast<float>(position.y),
            static_cast<float>(position.z)
        };
    }
    std::size_t UfbxModelSource::GetFaceCount(
        std::size_t meshIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        return mesh->num_faces;
    }
    //면을 구성하는 원본 정점 번호 목록
    std::vector<std::size_t> UfbxModelSource::GetFaceVertexIndices(
        std::size_t meshIndex,
        std::size_t faceIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
        const ufbx_face& face = mesh->faces.data[faceIndex];

        std::vector<std::size_t> indices;

        for (std::size_t i = 0; i < face.num_indices; ++i)
        {
            indices.push_back(
                mesh->vertex_indices.data[face.index_begin + i]
            );
        }

        return indices;
    }
    //다면체 분할
    std::vector<ModelTriangle> UfbxModelSource::TriangulateFace(
        std::size_t meshIndex,
        std::size_t faceIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
        const ufbx_face& face = mesh->faces.data[faceIndex];

        if (face.num_indices < 3)
            return {};

        std::vector<std::uint32_t> indices(
            (static_cast<std::size_t>(face.num_indices) - 2) * 3
        );

        const std::size_t triangleCount = ufbx_triangulate_face(
            indices.data(),
            indices.size(),
            mesh,
            face
        );

        std::vector<ModelTriangle> triangles;

        for (std::size_t i = 0; i < triangleCount; ++i)
        {
            const std::size_t offset = i * 3;

            triangles.push_back({
                mesh->vertex_indices.data[indices[offset]],
                mesh->vertex_indices.data[indices[offset + 1]],
                mesh->vertex_indices.data[indices[offset + 2]]
                });
        }

        return triangles;
    }
}

