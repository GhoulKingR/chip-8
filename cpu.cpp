#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>
#include <chrono>
#include <cstdbool>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cerrno>    
#include <SDL2/SDL.h>

#include "config.hpp"
#include "display.hpp"
#include "ram.hpp"
#include "sound.hpp"
#include "cpu.hpp"

static int key_translations[16] = {
    SDLK_0, SDLK_1,
    SDLK_2, SDLK_3,
    SDLK_4, SDLK_5,
    SDLK_6, SDLK_7,
    SDLK_8, SDLK_9,
    SDLK_a, SDLK_b,
    SDLK_c, SDLK_d,
    SDLK_e, SDLK_f
};

CPU::CPU(Display &display, Memory &memory, Sound &sound) :
    engine(std::chrono::system_clock::now().time_since_epoch().count()),
    dist(0x0, 0x99),
    display(display),
    memory(memory),
    sound(sound)
{}

void CPU::run_instruction() {
    uint8_t b12 = memory.ram[pc++];
    uint8_t b34 = memory.ram[pc++];
    uint8_t b1 = b12 >> 4;
    uint8_t b2 = b12 & 0xf;
    uint8_t b3 = b34 >> 4;
    uint8_t b4 = b34 & 0xf;

    if (b1 == 0) {
        if (b34 == 0xE0) {  // CLS
            display.clear();
        } else if (b34 == 0xEE) {  // RET
            pc = stack[sp--];
        }
    } else if (b1 == 1) {
        pc = 0;
        pc = b2 << 8;
        pc += b34;
    } else if (b1 == 2) {
        stack[++sp] = pc;
        pc = 0;
        pc = b2 << 8;
        pc += b34;
    } else if (b1 == 3) {
        if (v[b2] == b34) {
            pc += 2;
        }
    } else if (b1 == 4) {
        if (v[b2] != b34) {
            pc += 2;
        }
    } else if (b1 == 5) {
        if (v[b2] == v[b3]) {
            pc += 2;
        }
    } else if (b1 == 6) {
        v[b2] = b34;
    } else if (b1 == 7) {
        v[b2] += b34;
    } else if (b1 == 8) {
        if (b4 == 0) {
            v[b2] = v[b3];
        } else if (b4 == 1) {
            v[b2] |= v[b3];
        } else if (b4 == 2) {
            v[b2] &= v[b3];
        } else if (b4 == 3) {
            v[b2] ^= v[b3];
        } else if (b4 == 4) {
            uint16_t tmp = v[b2];
            tmp += (uint16_t ) v[b3];
            v[0xF] = tmp > 255 ? 1 : 0;
            v[b2] = (uint8_t) tmp;
        } else if (b4 == 5) {
            v[0xF] = v[b2] > v[b3] ? 1 : 0;
            v[b2] -= v[b3];
        } else if (b4 == 6) {
            v[0xF] = v[b2] % 2 == 1 ? 1 : 0;
            v[b2] /= 2;
        } else if (b4 == 7) {
            v[b2] = v[b3] - v[b2];
            v[0xF] = v[b3] > v[b2] ? 1 : 0;
        } else if (b4 == 0xE) {
            v[0xF] = v[b2] >> 7;
            v[b2] *= 2;
        }
    } else if (b1 == 9) {
        if (v[b2] != v[b3]) {
            pc += 2;
        }
    } else if (b1 == 0xA) {
        I = 0;
        I = b2 << 8;
        I += b34;
    } else if (b1 == 0xB) {
        pc = 0;
        pc = b2 << 8;
        pc += b34;
        pc += v[0];
    } else if (b1 == 0xC) {
        uint8_t randomByte = dist(engine) % 0x100;
        randomByte &= b34;
        v[b2] = randomByte;
    } else if (b1 == 0xD) {
        uint8_t* sprite = memory.ram.data() + I; // size b4
        display.write_to(v[b2], v[b3], sprite, b4, &(v[0xF]));
        display.render();
    } else if (b1 == 0xE) {
        if (b34 == 0x9E) {
            if (keys[key_translations[b2]]) {
                pc += 2;
            }
        } else if (b34 == 0xA1) {
            if (!keys[key_translations[b2]])
                pc += 2;
        }
    } else if (b1 == 0xF) {
        if (b34 == 0x07) {
            v[b2] = dt;
        } else if (b34 == 0x0A) {
            // printf("Forced key press\n");
            SDL_Event e;

            while (1) {
                if (SDL_WaitEvent(&e)) {
                    if (e.type == SDL_KEYDOWN) {
                        for (int i = 0; i < 16; i++) {
                            if (e.key.keysym.sym == key_translations[i]) {
                                v[b2] = i;
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
            dt = v[b2];
        } else if (b34 == 0x18) {
            st = v[b2];
        } else if (b34 == 0x1E) {
            I += v[b2];
        } else if (b34 == 0x29) {
            I = memory.sprite_locations[v[b2]];
        } else if (b34 == 0x33) {
            uint8_t val = v[b2];
            memory.ram[I + 2] = val % 10;
            val >>= 1;
            memory.ram[I + 1] = val % 10;
            val >>= 1;
            memory.ram[I] = val % 10;
        } else if (b34 == 0x55) {
            for (int i = 0; i <= b2; i++) {
                memory.ram[I + i] = v[i];
            }
        } else if (b34 == 0x65) {
            for (int i = 0; i <= b2; i++) {
                v[i] = memory.ram[I + i];
            }
        }
    }
}

void CPU::start() {
    DEBUG_LOG("Starting executable");

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

        // decrement st and dt
        if (st > 0) st--;
        if (dt > 0) dt--;
        
        run_instruction();

        if (st > 0) {
            sound.start();
        } else {
            sound.stop();
        }

        SDL_Delay(5);
    }
}
