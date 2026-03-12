#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "display.h"
#include "cpu.h"
#include "ram.h"
#include "sound.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    cpu_init();
    init_display();
    init_sound();
    init_ram();

    if (argc == 2) {
        load_game(argv[1]);
    } else {
        SEND_FAILED("Missing argument: must provide path to chip-8 executable");
    }

    start_cpu();
    return 0;
}
