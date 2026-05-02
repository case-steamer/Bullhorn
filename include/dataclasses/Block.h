#pragma once

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

struct Block
{
    int hour;
    int minute;
    struct Track
    {
        fs::path filepath;
        float volume;
        fs::path outputPath;
    };
    std::vector<Track> allTracks;
};
