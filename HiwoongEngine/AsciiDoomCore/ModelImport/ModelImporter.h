#pragma once

#include "IModelImporter.h"
#include <memory>

namespace Hiwoong
{
    class IModelSourceReader;
    class IMeshLoader;

    class ModelImporter final : public IModelImporter
    {
    public:
        ModelImporter(
            std::shared_ptr<const IModelSourceReader> sourceReader,
            std::shared_ptr<const IMeshLoader> meshLoader
        );

        ModelImportResult Import(
            const std::string& filePath
        ) const override;

    private:
        std::shared_ptr<const IModelSourceReader> sourceReader;
        std::shared_ptr<const IMeshLoader> meshLoader;
    };
}