#include <cstdlib>
#include <ctime>

#include "config.hpp"
#include "display.hpp"
#include "cpu.hpp"
#include "ram.hpp"
#include "sound.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        SEND_FAILED("Missing argument: must provide path to chip-8 executable");
    }

    Display display;
    Memory memory(argv[1]);
    CPU cpu(display, memory);
    init_sound();

    cpu.start();
    return 0;
}
