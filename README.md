# Chip-8 emulator

## Requirements
To compile this project, you need the following dependencies:
- SDL2
- Cmake 4.0+
- GCC/Clang
- make

## Compiling

To compile the emulator follow these steps:
1. Create a build directory and **cd** into it:
```bash
mkdir build
cd build
```
2. Create the Makefile and other required CMake files with the CMake command:
```bash
cmake ..
```
3. Compile the project:
```bash
make
```

This creates a `chip-8` executable file that you can run with following this syntax:
```bash
./chip-8 ../path/to/rom.ch8
```
