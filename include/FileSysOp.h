#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Queue.h"
class   Driver;

namespace fs = std::filesystem;

class FileSysOp
{
    public:
        FileSysOp(Driver& driver);
        bool isValid(const fs::path& input);
        bool isValid(const fs::path& input, const std::vector<std::string>& extensions);
        bool isValid(const fs::path& input, const Queue& queue);
        void deleteFile(fs::path& input);
        bool createNewProject() const;
        fs::path getMediaPath() const;

    private:
        Driver& driver;
        fs::path mediaPath;
};
