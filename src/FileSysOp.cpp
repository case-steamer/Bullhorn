#include "FileSysOp.h"

bool    FileSysOp::isValid(const fs::path& input)
{
    return fs::exists(input);
}

bool    FileSysOp::isValid(const fs::path& input, const std::vector<std::string>& extensions)
{
    if (!fs::exists(input)) return false;
    for (const std::string& suffix : extensions)
    {
        if (input.extension() == suffix) return true;
    }
    return false;
}

fs::path FileSysOp::getMediaPath() const
{
    return mediaPath;
}
