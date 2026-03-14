#include <fstream>
#include <ios>
#include <iosfwd>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.hpp"
#include "ram.hpp"


void Memory::load_sprites() {
    uint16_t pos = 0x0;

    // 0
    sprite_locations[0x0] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0x90;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;

    // 1
    sprite_locations[0x1] = pos;
    ram[pos++] = 0x20;
    ram[pos++] = 0x60;
    ram[pos++] = 0x20;
    ram[pos++] = 0x20;
    ram[pos++] = 0x70;

    // 2
    sprite_locations[0x2] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;

    // 3
    sprite_locations[0x3] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0xF0;

    // 4
    sprite_locations[0x4] = pos;
    ram[pos++] = 0x90;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0x10;

    // 5
    sprite_locations[0x5] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0xF0;
    
    // 6
    sprite_locations[0x6] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;

    // 7
    sprite_locations[0x7] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0x20;
    ram[pos++] = 0x40;
    ram[pos++] = 0x40;

    // 8
    sprite_locations[0x8] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;

    // 9
    sprite_locations[0x9] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x10;
    ram[pos++] = 0xF0;

    // A
    sprite_locations[0xA] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x90;
    ram[pos++] = 0x90;

    // B
    sprite_locations[0xB] = pos;
    ram[pos++] = 0xE0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xE0;
    ram[pos++] = 0x90;
    ram[pos++] = 0xE0;

    // C
    sprite_locations[0xC] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0x80;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;

    // D
    sprite_locations[0xD] = pos;
    ram[pos++] = 0xE0;
    ram[pos++] = 0x90;
    ram[pos++] = 0x90;
    ram[pos++] = 0x90;
    ram[pos++] = 0xE0;

    // E
    sprite_locations[0xE] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;

    // F
    sprite_locations[0xF] = pos;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0xF0;
    ram[pos++] = 0x80;
    ram[pos++] = 0x80;
}

Memory::Memory(const char *game_path) {
    DEBUG_LOG("Initializing ram");

    std::ifstream game_file(game_path, std::ios::in | std::ios::binary);
    if (!game_file) {
        SEND_FAILED("File \"%s\" does not exist", game_path);
    }

    // size
    game_file.seekg(0, std::ios::end);
    std::streampos size = game_file.tellg();
    game_file.seekg(0, std::ios::beg);

    uint8_t* loadptr = ram.data() + PROGRAM_BEGIN;
    game_file.read((char *) loadptr, size);

    load_sprites();
    game_file.close();
    DEBUG_LOG("Game loaded");
}
