#pragma once

#include "IModelSource.h"

struct ufbx_scene;

namespace Hiwoong
{
    class UfbxModelSource final : public IModelSource
    {
    public:
        explicit UfbxModelSource(ufbx_scene* scene);
        ~UfbxModelSource() override;

        UfbxModelSource(const UfbxModelSource&) = delete;
        UfbxModelSource& operator=(const UfbxModelSource&) = delete;

        std::size_t GetMeshCount() const override;

        std::string GetMeshName(
            std::size_t meshIndex
        ) const override;

        ModelVertex GetVertex(
            std::size_t meshIndex,
            std::size_t vertexIndex
        ) const override;

        std::size_t GetFaceCount(
            std::size_t meshIndex
        ) const override;

        std::vector<ModelTriangle> TriangulateFace(
            std::size_t meshIndex,
            std::size_t faceIndex
        ) const override;

        std::size_t GetCornerCount(
            std::size_t meshIndex
        ) const override;

        ModelVertex GetCornerVertex(
            std::size_t meshIndex,
            std::size_t cornerIndex
        ) const override;

        std::size_t GetCornerSourceVertexIndex(
            std::size_t meshIndex,
            std::size_t cornerIndex
        ) const override;

        std::vector<std::string> GetDiffuseTexturePaths(
            std::size_t meshIndex
        ) const override;

    private:
        ufbx_scene* scene;
    };
}

