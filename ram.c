#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"

static const int RAMSIZE = 4096;

uint8_t* ram = NULL;

static void exit_handler() {
    if (ram != NULL) {
        free(ram);
        ram = NULL;
    }
}

static void copy_sprites() {
    // TODO: Implement this
    // Copies sprites to memory but I need to
    // find out where programs expect this sprite
    // to be copied to
}

void init_ram() {
    ram = malloc(RAMSIZE);
    atexit(exit_handler);
    memset(ram, '\0', RAMSIZE);

    copy_sprites();
}
