#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <array>
#include <stddef.h>
#include <stdint.h>

#define DISPLAY_BUFFER_SIZE 32

class Display {
    std::array<uint64_t, DISPLAY_BUFFER_SIZE> screen{};
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

public:
    Display();
    ~Display();

    void clear();
    void render();
    void write_to(uint8_t x, uint8_t y, uint8_t* sprite, size_t n, uint8_t* carry);
};


#endif
