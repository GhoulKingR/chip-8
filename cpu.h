#ifndef CPU_H
#define CPU_H

#include <stdint.h>

struct cpu {
    // stack
    uint16_t stack[16];

    // Registers
    uint8_t v0;
    uint8_t v1;
    uint8_t v2;
    uint8_t v3;
    uint8_t v4;
    uint8_t v5;
    uint8_t v6;
    uint8_t v7;
    uint8_t v8;
    uint8_t v9;
    uint8_t vA;
    uint8_t vB;
    uint8_t vC;
    uint8_t vD;
    uint8_t vE;
    uint8_t vF;
    uint16_t I;

    // special purpose
    uint8_t dt;  // delay timer
    uint8_t st;  // sound timer
    uint16_t pc; // program counter
    uint8_t sp;  // stack pointer
};


void init_cpu(struct cpu*);
void start_cpu(struct cpu*);

#endif
