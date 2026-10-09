#include "ModelImporter.h"
#include "IModelSourceReader.h"
#include "IMeshLoader.h"
#include "ModelSourceReadResult.h"
#include "ModelImportContext.h"
#include "MeshLoadResult.h"
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

        result.model.meshes = std::move(meshResult.meshes);
        result.success = true;

        return result;
    }
}