#pragma once

#include <filesystem>

namespace fs = std::filesystem;

struct Project
{
    fs::path homeDir;
    fs::path media;
    fs::path blocks;
    fs::path queue;

    Project(const fs::path& root) : homeDir(root)
    {
        media   = homeDir/"media";
        blocks  = homeDir/"blocks";
        queue   = homeDir/"QUEUE.xml";
    }
};
