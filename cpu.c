#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>    
#include <SDL2/SDL.h>

#include "display.h"
#include "ram.h"
#include "cpu.h"

struct cpu cpu;
bool keys[322];

int key_translations[16] = {
    SDLK_0, SDLK_1,
    SDLK_2, SDLK_3,
    SDLK_4, SDLK_5,
    SDLK_6, SDLK_7,
    SDLK_8, SDLK_9,
    SDLK_a, SDLK_b,
    SDLK_c, SDLK_d,
    SDLK_e, SDLK_f
};

void init_cpu() {
    memset(&cpu, 0, sizeof(struct cpu));
    memset(keys, 0, 322 * sizeof(bool));
    cpu.sp = -1;
}

static void run_instruction(uint8_t b12, uint8_t b34) {
    uint8_t b1 = b12 >> 4;
    uint8_t b2 = b12 << 4;
    b2 >>= 4;
    uint8_t b3 = b34 >> 4;
    uint8_t b4 = b34 << 4;
    b4 >>= 4;

    if (b1 == 0) {
        if (b34 == 0xE0) {  // CLS
            display_clear();
        } else if (b34 == 0xEE) {  // RET
            cpu.pc = cpu.stack[cpu.sp--];
        }
    } else if (b1 == 1) {
        cpu.pc = 0;
        cpu.pc = b2 << 8;
        cpu.pc += b34;
    } else if (b1 == 2) {
        cpu.stack[++cpu.sp] = cpu.pc;
        cpu.pc = 0;
        cpu.pc = b2 << 8;
        cpu.pc += b34;
    } else if (b1 == 3) {
        if (cpu.v[b2] == b34) {
            cpu.pc += 2;
        }
    } else if (b1 == 4) {
        if (cpu.v[b2] != b34) {
            cpu.pc += 2;
        }
    } else if (b1 == 5) {
        if (cpu.v[b2] == cpu.v[b3]) {
            cpu.pc += 2;
        }
    } else if (b1 == 6) {
        cpu.v[b2] = b34;
    } else if (b1 == 7) {
        cpu.v[b2] += b34;
    } else if (b1 == 8) {
        if (b4 == 0) {
            cpu.v[b2] = cpu.v[b3];
        } else if (b4 == 1) {
            cpu.v[b2] |= cpu.v[b3];
        } else if (b4 == 2) {
            cpu.v[b2] &= cpu.v[b3];
        } else if (b4 == 3) {
            cpu.v[b2] ^= cpu.v[b3];
        } else if (b4 == 4) {
            uint16_t tmp = cpu.v[b2];
            tmp += (uint16_t ) cpu.v[b3];
            cpu.v[0xF] = tmp > 255 ? 1 : 0;
            cpu.v[b2] = (uint8_t) tmp;
        } else if (b4 == 5) {
            cpu.v[0xF] = cpu.v[b2] > cpu.v[b3] ? 1 : 0;
            cpu.v[b2] -= cpu.v[b3];
        } else if (b4 == 6) {
            cpu.v[0xF] = cpu.v[b2] % 2 == 1 ? 1 : 0;
            cpu.v[b2] /= 2;
        } else if (b4 == 7) {
            cpu.v[b2] = cpu.v[b3] - cpu.v[b2];
            cpu.v[0xF] = cpu.v[b3] > cpu.v[b2] ? 1 : 0;
        } else if (b4 == 0xE) {
            cpu.v[0xF] = cpu.v[b2] >> 7;
            cpu.v[b2] *= 2;
        }
    } else if (b1 == 9) {
        if (cpu.v[b2] != cpu.v[b3]) {
            cpu.pc += 2;
        }
    } else if (b1 == 0xA) {
        cpu.I = 0;
        cpu.I = b2 << 8;
        cpu.I += b34;
    } else if (b1 == 0xB) {
        cpu.pc = 0;
        cpu.pc = b2 << 8;
        cpu.pc += b34;
        cpu.pc += cpu.v[0];
    } else if (b1 == 0xC) {
        uint8_t randomByte = rand() % 0x100;
        randomByte &= b34;
        cpu.v[b2] = randomByte;
    } else if (b1 == 0xD) {
        uint8_t* sprite = ram + cpu.I; // size b4
        display_write_to(cpu.v[b2], cpu.v[b3], sprite, b4, cpu.v + 0xF);
        display_render();
    } else if (b1 == 0xE) {
        if (b34 == 0x9E) {
            if (keys[key_translations[b2]]) {
                cpu.pc += 2;
            }
        } else if (b34 == 0xA1) {
            if (!keys[key_translations[b2]])
                cpu.pc += 2;
        }
    } else if (b1 == 0xF) {
        if (b34 == 0x07) {
            cpu.v[b2] = cpu.dt;
        } else if (b34 == 0x0A) {
            // printf("Forced key press\n");
            SDL_Event e;

            while (1) {
                if (SDL_WaitEvent(&e)) {
                    if (e.type == SDL_KEYDOWN) {
                        for (int i = 0; i < 16; i++) {
                            if (e.key.keysym.sym == key_translations[i]) {
                                cpu.v[b2] = i;
                                // printf("Key %d pressed\n", i);
                                return;
                            }
                        }
                    } else if (e.type == SDL_QUIT) {
                        exit(0);
                    }
                }
            }
        } else if (b34 == 0x15) {
            cpu.dt = cpu.v[b2];
        } else if (b34 == 0x18) {
            cpu.st = cpu.v[b2];
        } else if (b34 == 0x1E) {
            cpu.I += cpu.v[b2];
        } else if (b34 == 0x29) {
            cpu.I = sprite_locations[cpu.v[b2]];
        } else if (b34 == 0x33) {
            uint8_t val = cpu.v[b2];
            ram[cpu.I + 2] = val % 10;
            val >>= 1;
            ram[cpu.I + 1] = val % 10;
            val >>= 1;
            ram[cpu.I] = val % 10;
        } else if (b34 == 0x55) {
            for (int i = 0; i <= b2; i++) {
                ram[cpu.I + i] = cpu.v[i];
            }
        } else if (b34 == 0x65) {
            for (int i = 0; i <= b2; i++) {
                cpu.v[i] = ram[cpu.I + i];
            }
        }
    }
}

void start_cpu() {
    // clock
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = false;
                    return;
                case SDL_KEYDOWN:
                    keys[e.key.keysym.sym] = true;
                    break;
                case SDL_KEYUP:
                    keys[e.key.keysym.sym] = false;
                    break;
            }
        }

        run_instruction(ram[cpu.pc++], ram[cpu.pc++]);

        // decrement st and dt
        if (cpu.st > 0) cpu.st--;
        if (cpu.dt > 0) cpu.dt--;

        SDL_Delay(5);
    }
}
