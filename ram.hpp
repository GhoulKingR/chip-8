#ifndef RAM_H
#define RAM_H

#include "config.hpp"
#include "cpu.hpp"
#include <array>
#include <cstdint>
#include <stdint.h>

struct Memory {
    std::array<uint8_t, RAMSIZE> ram{};

    uint16_t sprite_locations[16];
    Memory(CPU *cpu, const char *game_path);
    void load_sprites();
};

#endif
