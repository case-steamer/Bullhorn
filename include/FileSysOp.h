#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class FileSysOp
{
    public:
        bool isValid(const fs::path& input);
        bool isValid(const fs::path& input, const std::vector<std::string>& extensions);
        fs::path getMediaPath() const;

    private:
        fs::path mediaPath;
};
