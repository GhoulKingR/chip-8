#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct cpu {
    // stack
    uint16_t stack[16];

    // Registers
    uint8_t v[16];
    uint16_t I;

    // special purpose
    uint8_t dt;  // delay timer
    uint8_t st;  // sound timer
    uint16_t pc; // program counter
    uint8_t sp;  // stack pointer
};

extern struct cpu cpu;

void init_cpu();
void start_cpu();

#endif
