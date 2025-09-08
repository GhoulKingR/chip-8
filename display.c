#include "display.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

static struct {
    uint64_t screen[32];
} display;
static const int DISPLAY_WIDTH = 512;       // scale 8 : 1
static const int DISPLAY_HEIGHT = 256;      // scale 8 : 1

static void exit_handler() {
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}

void init_display() {
    memset(display.screen, 0, 32 * sizeof(uint64_t));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    atexit(exit_handler);

    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DISPLAY_WIDTH, DISPLAY_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

static void clean_render_view() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void display_render() {
    SDL_Rect rects[64 * 32];
    int size = 0;

    for (size_t i = 0; i < 32; i++) {
        uint64_t line = display.screen[i];
        for (int j = 63; j >= 0; j--) {
            uint8_t state = line % 2;
            line >>= 1;
            if (state) {
                SDL_Rect r;
                r.w = 8; r.h = 8;
                r.x = j * 8; r.y = i * 8;
                rects[size++] = r;
            }
        }
    }

    clean_render_view();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRects(renderer, rects, size);
    SDL_RenderPresent(renderer);
}

void display_clear() {
    clean_render_view();
    memset(display.screen, 0, 32 * sizeof(uint64_t));
    display_render();
}

void display_write_to(uint8_t x, uint8_t y, uint8_t* sprite, size_t n, uint8_t* carry) {
    *carry = 0;

    for (int i = 0; i < n; i++) {
        uint64_t sprite_line = 0;
        sprite_line += sprite[i];
        sprite_line <<= 56;
        sprite_line >>= x;
        uint64_t display_line = display.screen[y + i];

        uint64_t c1 = display_line;
        display_line ^= sprite_line;
        display.screen[y + i] = display_line;
        uint64_t c2 = display_line;

        if (*carry == 0) {
            for (int i = 0; i < 64; i++) {
                uint8_t b1 = c1 % 10, b2 = c2 % 10;
                c1 >>= 1; c2 >>= 1;
                if (b1 == 1 && b2 == 0) {
                    *carry = 1;
                    return;
                }
            }
        }
    }
}
