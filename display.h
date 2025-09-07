#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

struct display {
    uint64_t screen[32];
};

void init_display(struct display*);

extern SDL_Window* window;
extern SDL_Renderer* renderer;

#endif
