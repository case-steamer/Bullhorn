#define MINIAUDIO_IMPLEMENTATION
#include "AudioPlayer.h"

#include <iostream>
#include <chrono>
#include <thread>

void AudioPlayer::playTrack(const fs::path& filepath)
{
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        std::cout << "Audio failure" << std::endl;
        return;
    }

    ma_engine_play_sound(&engine, filepath.c_str(), NULL);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ma_engine_uninit(&engine);
}
