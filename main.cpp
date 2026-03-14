#include <cstdlib>
#include <ctime>

#include "config.hpp"
#include "display.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "sound.hpp"

int main(int argc, char** argv) {
    Display display;
    CPU cpu(&display);
    init_sound();

    if (argc < 2) {
        SEND_FAILED("Missing argument: must provide path to chip-8 executable");
    }

    Memory memory(&cpu, argv[1]);

    cpu.start();
    return 0;
}
