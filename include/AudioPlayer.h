#pragma once

#include "miniaudio.h"
#include <filesystem>

namespace fs = std::filesystem;

class AudioPlayer
{
    private:
        ma_result result;
        ma_engine engine;

    public:
        void playTrack(const fs::path& filepath);
};
