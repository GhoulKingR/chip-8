#include "display.h"
#include "config.h"

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
} display = {0};

static void exit_handler() {
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}


void init_display() {
    DEBUG_LOG("Initializing display");
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SEND_FAILED("Error initializing SDL: %s", SDL_GetError());
    } else {
        DEBUG_LOG("SDL initialized successfully");
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
        SEND_FAILED("Error creating window: %s", SDL_GetError());
    } else {
        DEBUG_LOG("Window created successfully");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SEND_FAILED("Error creating renderer: %s", SDL_GetError());
    } else {
        DEBUG_LOG("Render created successfully");
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
}

void display_write_to(uint8_t x, uint8_t y, uint8_t* sprite, size_t n, uint8_t* carry) {
    *carry = 0;

    for (int i = 0; i < n; i++) {
        uint64_t sprite_line = 0;
        sprite_line += sprite[i];
        sprite_line <<= 56;
        sprite_line >>= x;

        uint64_t c1 = display.screen[y + i];
        display.screen[y + i] ^= sprite_line;
        uint64_t c2 = display.screen[y + i];

        if (*carry == 0) {
            for (int j = 0; j < 64; j++) {
                uint8_t b1 = c1 % 0b10, b2 = c2 % 0b10;
                c1 >>= 1; c2 >>= 1;
                if (b1 == 1 && b2 == 0) {
                    *carry = 1;
                    break;
                }
            }
        }
    }
}
