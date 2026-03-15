#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_audio.h>
#include <cstdint>

class Sound {
    SDL_AudioDeviceID audioDevice;
    bool playing = false;

    constexpr static double m_volume = 1.0;
    constexpr static double m_frequency = 261.63;
    static void (*writeData)(uint8_t* ptr, double data);
    static int (*calculateOffset)(int sample, int channel);
    static SDL_AudioSpec obtainedSpec;
    static int m_pos;
    static int calculateOffset_s16(int sample, int channel);
    static int calculateOffset_f32(int sample, int channel);
    static void writeData_s16(uint8_t* ptr, double data);
    static void writeData_f32(uint8_t* ptr, double data);
    static double getData();
    static void audioCallback(void* userdata, uint8_t* stream, int len);

public:
    Sound();
    void start();
    void stop();
};

#endif

