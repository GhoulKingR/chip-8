#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"
#define PROGRAM_BEGIN 0x200

static const int RAMSIZE = 4096;

uint8_t* ram = NULL;
uint16_t sprite_locations[16];

static void exit_handler() {
    if (ram != NULL) {
        free(ram);
        ram = NULL;
    }
}

static void load_sprites() {
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

void init_ram() {
    ram = malloc(RAMSIZE);
    atexit(exit_handler);
    memset(ram, '\0', RAMSIZE);

    load_sprites();
}

void load_game(char* game_path) {
    FILE* fptr = fopen(game_path, "rb");
    uint8_t* loadptr = ram + PROGRAM_BEGIN;
    cpu.pc = PROGRAM_BEGIN;
    fread(loadptr, 1, 4096 - PROGRAM_BEGIN, fptr);
}
