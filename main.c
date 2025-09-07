#include <stdint.h>
#include <stdlib.h>

#include "display.h"
#include "cpu.h"
#include "ram.h"

int main(int argc, char** argv) {
    init_ram();

    struct display display;
    init_display(&display);

    struct cpu cpu;
    init_cpu(&cpu);
    start_cpu(&cpu);

    return 0;
}
