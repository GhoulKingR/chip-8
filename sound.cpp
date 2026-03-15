#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound.hpp"
#include "config.hpp"

void (*Sound::writeData)(uint8_t* ptr, double data) = nullptr;
int (*Sound::calculateOffset)(int sample, int channel) = nullptr;
SDL_AudioSpec Sound::obtainedSpec = {};
int Sound::m_pos = 0;

// ---
// Calculate the offset in bytes from the start of the audio stream to the
// memory address at `sample` and `channel`.
//
// Channels are interleaved.

int Sound::calculateOffset_s16(int sample, int channel) {
    return
        (sample * sizeof(int16_t) * obtainedSpec.channels) +
        (channel * sizeof(int16_t));
}

int Sound::calculateOffset_f32(int sample, int channel) {
    return
        (sample * sizeof(float) * obtainedSpec.channels) +
        (channel * sizeof(float));
}

// ---
// Convert a normalized data value (range: 0.0 .. 1.0) to a data value matching
// the audio format.

void Sound::writeData_s16(uint8_t* ptr, double data) {
    int16_t* ptrTyped = (int16_t*)ptr;
    double range = (double)INT16_MAX - (double)INT16_MIN;
    double dataScaled = data * range / 2.0;
    *ptrTyped = dataScaled;
}

void Sound::writeData_f32(uint8_t* ptr, double data) {
    float* ptrTyped = (float*)ptr;
    *ptrTyped = data;
}

double Sound::getData() {
    double sampleRate = (double) obtainedSpec.freq;
    double period = sampleRate / m_frequency;

    if (m_pos % (int)period == 0) {
        m_pos = 0;
    }

    double pos = m_pos;
    double angular_freq = (1.0 / period) * 2.0 * M_PI;
    double amplitude = m_volume;

    return sin(pos * angular_freq) * amplitude;
}

void Sound::audioCallback(void* userdata, uint8_t* stream, int len) {
    (void)userdata;
    (void)len;

    for (int sample = 0; sample < obtainedSpec.samples; sample++) {
        double data = getData();
        m_pos++;

        // write data to channels
        for (int channel = 0; channel < obtainedSpec.channels; channel++) {
            int offset = calculateOffset(sample, channel);
            uint8_t* ptrData = stream + offset;
            writeData(ptrData, data);
        }
    }
}

Sound::Sound() {
    DEBUG_LOG("Initializing sound");
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);

    // common sampling frequency
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16;
    desiredSpec.samples = 512;
    desiredSpec.channels = 1;
    desiredSpec.callback = audioCallback;

    audioDevice = SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, 0);

    if (audioDevice == 0) {
        SEND_FAILED("Failed to open audio: %s\n", SDL_GetError());
    } else {
        char formatName[10];
        switch (obtainedSpec.format) {
            case AUDIO_S16:
                writeData = writeData_s16;
                calculateOffset = calculateOffset_s16;
                strcpy(formatName, "AUDIO_S16");
                break;
            case AUDIO_F32:
                writeData = writeData_f32;
                calculateOffset = calculateOffset_f32;
                strcpy(formatName, "AUDIO_F32");
                break;
            default:
                SEND_FAILED("Unsupported audio format: %i\n", obtainedSpec.format);
                break;
        }
    }
}


void Sound::start() {
    if (!playing) {
        SDL_PauseAudioDevice(audioDevice, 0);
        playing = true;
    }
}

void Sound::stop() {
    if (playing) {
        SDL_PauseAudioDevice(audioDevice, 1);
        playing = false;
    }
}
