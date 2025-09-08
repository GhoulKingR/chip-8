#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "display.h"
#include "cpu.h"
#include "ram.h"
#include "sound.h"

int main(int argc, char** argv) {
    srand(time(NULL));
    init_display();
    init_cpu();
    init_sound();

    init_ram();
    if (argc == 2)
        load_game(argv[1]);
    else {
        fprintf(stderr, "Missing argument: must provide path to game file\n");
        return EXIT_FAILURE;
    }


    start_cpu();
    return 0;
}
