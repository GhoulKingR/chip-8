#ifndef CPU_H
#define CPU_H

#include "config.hpp"
#include "display.hpp"
#include "ram.hpp"
#include <array>
#include <cstdint>
#include <random>
#include <unordered_map>

struct CPU {
    // stack
    std::array<uint16_t, 16> stack{};

    // Registers
    std::array<uint8_t, 16> v{};
    uint16_t I = 0;

    // special purpose
    uint8_t dt = 0;     // delay timer
    uint8_t st = 0;     // sound timer
    uint16_t pc = PROGRAM_BEGIN;    // program counter
    uint8_t sp = -1;    // stack pointer

    std::mt19937 engine;
    std::uniform_int_distribution<int> dist;

    Display &display;
    Memory &memory;

    CPU(Display&, Memory&);
    void start();

private:
    std::unordered_map<int, bool> keys;
    void run_instruction();
};


#endif
