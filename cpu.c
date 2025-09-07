#include <SDL2/SDL_events.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>    
#include <SDL2/SDL.h>

#include "display.h"
#include "cpu.h"

void init_cpu(struct cpu *cpu) {
    memset(cpu, 0, sizeof(struct cpu));
}

void start_cpu(struct cpu* cpu) {
    // clock
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    return;
            }
        }

        // decrement st and dt
        if (cpu->st > 0) cpu->st--;
        if (cpu->dt > 0) cpu->dt--;

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}
