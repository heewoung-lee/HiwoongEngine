#pragma once

#include "ModelImportResult.h"
#include <string>

namespace Hiwoong
{
    class IModelImporter
    {
    public:
        virtual ~IModelImporter() = default;

        virtual ModelImportResult Import(
            const std::string& filePath
        ) const = 0;

        virtual bool ImportAnimation(
            ModelData& model,
            const std::string& filePath,
            const std::string& name
        ) const = 0;
    };
}