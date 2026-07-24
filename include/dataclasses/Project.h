#pragma once

#include <filesystem>

namespace fs = std::filesystem;

struct Project
{
    const fs::path homeDir;
    const fs::path media;
    const fs::path blocks;
    const fs::path queue;

    Project(const fs::path& root) :
        homeDir(root), 
        media(homeDir/"media"),
        blocks(homeDir/"blocks"),
        queue(homeDir/"QUEUE.xml")
    {
    }
};
