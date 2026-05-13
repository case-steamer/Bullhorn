#define MINIAUDIO_IMPLEMENTATION
#include "AudioPlayer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>

AudioPlayer::AudioPlayer()
{
    ma_engine_init(NULL, &engine); 
    if (result != MA_SUCCESS)
    {
        throw std::runtime_error("Audio engine failed to initialize.");
    }

}

void AudioPlayer::playTrack(const fs::path& filepath)
{
    ma_sound sound;
    ma_sound_init_from_file(&engine, filepath.c_str(), 0, NULL, NULL, &sound);

   
    ma_sound_start(&sound);
    float sTime;
    ma_sound_get_length_in_seconds(&sound, &sTime);
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(sTime * 1000)));


    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}

AudioPlayer::~AudioPlayer()
{
    ma_engine_uninit(&engine);
}
