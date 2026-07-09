#pragma once

#include "miniaudio.h"
#include <filesystem>
#include <mutex>
#include <atomic>

namespace fs = std::filesystem;

class AudioPlayer
{
    private:
        ma_result result;
        ma_engine engine;
        std::mutex audioMutex;
        std::atomic<bool> stopPlayback{false};

    public:
        AudioPlayer();
        void interrupt();
        void playTrack(const fs::path& filepath, float volume);
        void playBeep();
        bool isPlaying();
        ~AudioPlayer();
};
