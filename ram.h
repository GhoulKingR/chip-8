#ifndef RAM_H
#define RAM_H

#include "cpu.h"
#include <stdint.h>

void init_ram();
void load_game(char*);

extern uint8_t* ram;
extern uint16_t sprite_locations[16];

#endif
