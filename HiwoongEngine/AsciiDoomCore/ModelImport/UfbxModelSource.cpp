#include "UfbxModelSource.h"
#include "../ThirdParty/ufbx.h"
#include <filesystem>
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

        std::size_t materialIndex = 0;

        if (faceIndex < mesh->face_material.count)
        {
            materialIndex = mesh->face_material.data[faceIndex];
        }

        for (std::size_t i = 0; i < triangleCount; ++i)
        {
            const std::size_t offset = i * 3;
                   triangles.push_back({
                   indices[offset],
                   indices[offset + 1],
                   indices[offset + 2],
                   materialIndex});
                }

        return triangles;
    }
    std::size_t UfbxModelSource::GetCornerCount(std::size_t meshIndex) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];
        return mesh->num_indices;
    }
    ModelVertex UfbxModelSource::GetCornerVertex(
        std::size_t meshIndex,
        std::size_t cornerIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        assert(cornerIndex < mesh->num_indices);

        // 이 코너가 사용하는 원본 정점 번호.
        const std::size_t vertexIndex =
            mesh->vertex_indices.data[cornerIndex];

        // 기존 함수로 위치를 가져온다.
        ModelVertex vertex = GetVertex(meshIndex, vertexIndex);

        if (mesh->vertex_uv.exists)
        {
            const ufbx_vec2 uv = ufbx_get_vertex_vec2(
                &mesh->vertex_uv,
                cornerIndex
            );

            vertex.u = static_cast<float>(uv.x);
            vertex.v = static_cast<float>(uv.y);
        }

        return vertex;
    }
    std::size_t UfbxModelSource::GetCornerSourceVertexIndex(
        std::size_t meshIndex,
        std::size_t cornerIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        assert(cornerIndex < mesh->num_indices);

        return mesh->vertex_indices.data[cornerIndex];
    }
    std::vector<std::string> UfbxModelSource::GetDiffuseTexturePaths(
        std::size_t meshIndex
    ) const
    {
        const ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        assert(mesh->instances.count == 1);
        const ufbx_node* node = mesh->instances.data[0];

        // 재질 개수만큼 빈 문자열을 만들어 번호를 유지한다.
        std::vector<std::string> paths(node->materials.count);

        for (std::size_t i = 0; i < node->materials.count; ++i)
        {
            const ufbx_material* material = node->materials.data[i];
            const ufbx_texture* texture =
                material->fbx.diffuse_color.texture;

            if (texture == nullptr)
            {
                texture = material->pbr.base_color.texture;
            }
            if (texture != nullptr && texture->filename.length > 0)
            {
                // fbxPath 생성
                const std::filesystem::path fbxPath(
                    std::string(
                        scene->metadata.filename.data,
                        scene->metadata.filename.length
                    )
                );
                // imagePath 생성
                const std::filesystem::path imagePath(
                    std::string(
                        texture->filename.data,
                        texture->filename.length
                    )
                );
                // paths[i] 대입
                paths[i] = (
                    fbxPath.parent_path() / "textures" / imagePath.filename()
                    ).string();

            }
           
        }

        return paths;
    }
}

