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

bool    FileSysOp::isValid(const fs::path& input, const Queue& queue)
{
    if (!fs::exists(input)) return false;
    for (Queue::BlockEntry ent : queue.allBlocks)
    {
        if (input == ent.filepath) return true;
    }
    return false;
}

fs::path FileSysOp::getMediaPath() const
{
    return mediaPath;
}
