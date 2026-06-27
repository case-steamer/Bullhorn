#pragma once

#include <vector>
#include <filesystem>

#include "Block.h"

namespace fs = std::filesystem;

struct Queue
{
    struct BlockEntry
    {
        fs::path filepath;
        Block block;
        bool isOverride = false;
    };
    std::vector<BlockEntry> allBlocks;
};
