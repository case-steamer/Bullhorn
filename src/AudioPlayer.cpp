#define MINIAUDIO_IMPLEMENTATION
#include "AudioPlayer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <stdexcept>

AudioPlayer::AudioPlayer()
{
    result = ma_engine_init(NULL, &engine); 
    if (result != MA_SUCCESS)
    {
        throw std::runtime_error("Audio engine failed to initialize.");
    }

}

void AudioPlayer::interrupt()
{
    stopPlayback = true;
}

void AudioPlayer::playTrack(const fs::path& filepath, float volume)
{
    std::lock_guard<std::mutex> guard(audioMutex);
    stopPlayback = false;

    ma_sound sound;
    ma_sound_init_from_file(&engine, filepath.c_str(), 0, NULL, NULL, &sound);
    ma_sound_set_volume(&sound, volume);
    ma_sound_start(&sound);

    float sTime;
    ma_sound_get_length_in_seconds(&sound, &sTime);

    auto duration   =   std::chrono::milliseconds((int)(sTime*1000));
    auto elapsed    =   std::chrono::milliseconds(0);
    auto interval   =   std::chrono::milliseconds(100);

    while (elapsed < duration && !stopPlayback)
        {
            std::this_thread::sleep_for(interval);
            elapsed += interval;
        }

    ma_sound_uninit(&sound);
}

void AudioPlayer::playBeep()
{
    ma_waveform_config config = ma_waveform_config_init(
            ma_format_f32,
            ma_engine_get_channels(&engine),
            ma_engine_get_sample_rate(&engine),
            ma_waveform_type_square,
            0.5,
            800
        );
    ma_waveform waveform;
    ma_result beepResult = ma_waveform_init(&config, &waveform);

    ma_sound sound;
    ma_sound_init_from_data_source(&engine, &waveform, 0, NULL, &sound);
    ma_sound_set_volume(&sound, 0.5f);
    ma_sound_start(&sound);


    auto duration   =   std::chrono::milliseconds((int)(75));
    auto elapsed    =   std::chrono::milliseconds(0);
    auto interval   =   std::chrono::milliseconds(100);

    while (elapsed < duration && !stopPlayback)
        {
            std::this_thread::sleep_for(interval);
            elapsed += interval;
        }

    ma_sound_uninit(&sound);
    ma_waveform_uninit(&waveform);
}

bool AudioPlayer::isPlaying()
{
    if (audioMutex.try_lock())
    {
        audioMutex.unlock();
        return false;
    }
    return true;
}

AudioPlayer::~AudioPlayer()
{
    ma_engine_uninit(&engine);
}
