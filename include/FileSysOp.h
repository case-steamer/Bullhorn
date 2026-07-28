#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <optional>

#include "Project.h"
#include "Queue.h"
class   Driver;

namespace fs = std::filesystem;

class FileSysOp
{
    public:
        FileSysOp(Driver& driver);
        bool isValid(const fs::path& input) const;
        bool isValid(const fs::path& input, const std::vector<std::string>& extensions) const;
        bool isValid(const fs::path& input, const Queue& queue) const;
        bool isValidBlockName(const fs::path& input, char identifier) const;
        std::optional<int>  nextBlockID(const Queue& queue, char identifier) const;
        void deleteFile(const fs::path& input);
        std::optional<Project> createNewProject(const fs::path& root) const;
        std::optional<Project> loadProject(const fs::path& root) const;
        fs::path getMediaPath() const;

    private:
        Driver& driver;
        fs::path mediaPath;
};
