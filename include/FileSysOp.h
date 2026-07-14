#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Queue.h"

namespace fs = std::filesystem;

class FileSysOp
{
    public:
        bool isValid(const fs::path& input);
        bool isValid(const fs::path& input, const std::vector<std::string>& extensions);
        bool isValid(const fs::path& input, const Queue& queue);
        void deleteFile(fs::path& input);
        fs::path getMediaPath() const;

    private:
        fs::path mediaPath;
};
