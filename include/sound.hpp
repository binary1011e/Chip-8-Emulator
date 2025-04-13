#pragma once
#include <cstdint>
#include <SDL_audio.h>
#include <cmath>
#include <iostream>

class Sound
{
public:
    Sound();
    ~Sound();
    void play();
    void stop();

    constexpr double m_sineFreq;
    constexpr double m_sampleFreq;
    constexpr double m_samplesPerSine;
    uint32_t m_samplePos;

private:
    static void SDLAudioCallback(void *data, Uint8 *buffer, int length);

    SDL_AudioDeviceID m_device;
};