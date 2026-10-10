#include "ModelImporter.h"
#include "IModelSourceReader.h"
#include "IMeshLoader.h"
#include "ModelSourceReadResult.h"
#include "ModelImportContext.h"
#include "MeshLoadResult.h"
#include "PngTextureLoader.h"
#include <algorithm>
#include <limits>
#include <utility>
#include <cassert>

namespace Hiwoong
{
    ModelImporter::ModelImporter(
        std::shared_ptr<const IModelSourceReader> sourceReader,
        std::shared_ptr<const IMeshLoader> meshLoader
    )
        : sourceReader(sourceReader),
        meshLoader(meshLoader)
    {
        assert(this->sourceReader != nullptr);
        assert(this->meshLoader != nullptr);
    }

    ModelImportResult ModelImporter::Import(
        const std::string& filePath
    ) const
    {
        ModelImportResult result;

        const ModelSourceReadResult readResult =
            sourceReader->Read(filePath);

        if (!readResult.success || readResult.source == nullptr)
        {
            result.errorMessage = readResult.errorMessage;
            return result;
        }

        ModelImportContext context;
        context.source = readResult.source;

        MeshLoadResult meshResult = meshLoader->Load(context);

        if (!meshResult.success)
        {
            result.errorMessage = meshResult.errorMessage;
            return result;
        }

        for (ModelMesh& mesh : meshResult.meshes)
        {
            mesh.diffuseTextures.resize(mesh.diffuseTexturePaths.size());

            for (std::size_t i = 0;
                i < mesh.diffuseTexturePaths.size();
                ++i)
            {
                const std::string& texturePath = mesh.diffuseTexturePaths[i];

                if (texturePath.empty())
                    continue;

                auto texture = std::make_shared<ModelTexture>();

                if (!LoadPngTexture(texturePath, *texture))
                {
                    result.errorMessage =
                        "Failed to load texture: " + texturePath;

                    return result;
                }

                mesh.diffuseTextures[i] = texture;
            }
        }

        result.model.meshes = std::move(meshResult.meshes);

        CenterModel(result.model);

        result.success = true;

        return result;
    }
    void ModelImporter::CenterModel(ModelData& model) const
    {
        const float largest = std::numeric_limits<float>::max();

        ModelVertex minPosition{ largest, largest, largest };
        ModelVertex maxPosition{ -largest, -largest, -largest };

        for (const ModelMesh& mesh : model.meshes)
        {
            for (const ModelVertex& vertex : mesh.vertices)
            {
                minPosition.x = std::min(minPosition.x, vertex.x);
                minPosition.y = std::min(minPosition.y, vertex.y);
                minPosition.z = std::min(minPosition.z, vertex.z);

                maxPosition.x = std::max(maxPosition.x, vertex.x);
                maxPosition.y = std::max(maxPosition.y, vertex.y);
                maxPosition.z = std::max(maxPosition.z, vertex.z);
            }
        }

        const ModelVertex center{
            (minPosition.x + maxPosition.x) * 0.5f,
            (minPosition.y + maxPosition.y) * 0.5f,
            (minPosition.z + maxPosition.z) * 0.5f
        };

        for (ModelMesh& mesh : model.meshes)
        {
            for (ModelVertex& vertex : mesh.vertices)
            {
                vertex.x -= center.x;
                vertex.y -= center.y;
                vertex.z -= center.z;
            }
        }
    }
}