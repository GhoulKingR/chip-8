#include <SDL2/SDL.h>
#include <cstdlib>
#include <ctime>

#include "config.hpp"
#include "display.hpp"
#include "ram.hpp"
#include "sound.hpp"
#include "cpu.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        SEND_FAILED("Missing argument: must provide path to chip-8 executable");
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SEND_FAILED("Error initializing SDL: %s", SDL_GetError());
    } else {
        DEBUG_LOG("SDL initialized successfully");
    }

    Sound sound;
    Display display;
    Memory memory(argv[1]);
    CPU cpu(display, memory, sound);

    cpu.start();
    return 0;
}
