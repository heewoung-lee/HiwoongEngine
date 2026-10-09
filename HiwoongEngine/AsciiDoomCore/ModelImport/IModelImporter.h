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
    };
}