#pragma once
#include <string>
#include <vector>

namespace FileUtil
{
    std::vector<std::string> LoadTextLines(
        const std::string& path
    );
}

