#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stddef.h>
#include <stdint.h>


void init_display();
void display_clear();
void display_write_to(uint8_t x, uint8_t y, uint8_t* sprite, size_t n, uint8_t* carry);
void display_render();

extern SDL_Window* window;
extern SDL_Renderer* renderer;

#endif
