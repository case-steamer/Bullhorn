#pragma once

#include <filesystem>
#include <string>
#include <array>

namespace fs = std::filesystem;

class FileSysOp
{
    public:
        bool isValid(const std::string& input);
        bool isValid(const fs::path& input);
        fs::path getMediaPath() const;

    private:
        fs::path mediaPath;
        const std::array<std::string, 4> supportedTypes = {".mp3", ".wav", ".flac", ".m4a"};
};
