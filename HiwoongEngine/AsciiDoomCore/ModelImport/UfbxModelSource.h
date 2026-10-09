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

        std::size_t GetVertexCount(
            std::size_t meshIndex
        ) const override;

        ModelVertex GetVertex(
            std::size_t meshIndex,
            std::size_t vertexIndex
        ) const override;

        std::size_t GetFaceCount(
            std::size_t meshIndex
        ) const override;

        std::vector<std::size_t> GetFaceVertexIndices(
            std::size_t meshIndex,
            std::size_t faceIndex
        ) const override;

        std::vector<ModelTriangle> TriangulateFace(
            std::size_t meshIndex,
            std::size_t faceIndex
        ) const override;

    private:
        ufbx_scene* scene;
    };
}

