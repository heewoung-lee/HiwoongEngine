#include "MapPropObject.h"
#include "ModelImport/ModelData.h"
#include "ModelImport/ModelMeshConverter.h"
#include "Component/MeshRenderComponent.h"
#include <cassert>

namespace Hiwoong
{
    MapPropObject::MapPropObject(
        const Vector3& position,
        const std::shared_ptr<const ModelData>& model
    )
        : GameObject(position),
        model(model)
    {
        assert(this->model != nullptr);
    }

    void MapPropObject::Start()
    {
        super::Start();
        InitMeshRenderer();
        InitCollider();

        const auto collider = GetComponent<BoxCollider3DComponent>();

        Vector3 position = transform->GetLocalPosition();
        position.y -= collider->GetHalfExtents().y;
        transform->SetLocalPosition(position);
    }

    void MapPropObject::InitMeshRenderer()
    {
        assert(!model->meshes.empty());

        for (const ModelMesh& modelMesh : model->meshes)
        {
            std::size_t materialCount =
                modelMesh.diffuseTextures.size();

            if (materialCount == 0)
                materialCount = 1;

            for (std::size_t i = 0; i < materialCount; ++i)
            {
                Mesh mesh = ConvertToRenderMesh(modelMesh, i);

                if (mesh.triangles.empty())
                    continue;

                std::shared_ptr<const ModelTexture> texture;

                if (i < modelMesh.diffuseTextures.size())
                    texture = modelMesh.diffuseTextures[i];

                AddComponent<MeshRenderComponent>(
                    mesh,
                    Color::White,
                    texture
                );
            }
        }
    }
    void MapPropObject::InitCollider()
    {
        Vector3 halfExtents = Vector3::Zero;

        for (const ModelMesh& mesh : model->meshes)
        {
            for (const ModelVertex& vertex : mesh.vertices)
            {
                halfExtents.x =
                    std::max(halfExtents.x, std::abs(vertex.x));
                halfExtents.y =
                    std::max(halfExtents.y, std::abs(vertex.y));
                halfExtents.z =
                    std::max(halfExtents.z, std::abs(vertex.z));
            }
        }

        assert(
            halfExtents.x > 0.0f &&
            halfExtents.y > 0.0f &&
            halfExtents.z > 0.0f
        );

        AddComponent<BoxCollider3DComponent>(halfExtents);
    }
}