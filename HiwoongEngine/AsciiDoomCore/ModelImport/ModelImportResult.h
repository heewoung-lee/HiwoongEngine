#pragma once

#include "ModelData.h"
#include <string>

namespace Hiwoong
{
    struct ModelImportResult
    {
        bool success = false;
        ModelData model;
        std::string errorMessage;
    };
}