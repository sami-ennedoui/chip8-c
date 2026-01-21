#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include "../src/chip8.h"

#define DISPLAY_WIDTH  64
#define DISPLAY_HEIGHT 32
#define SCALE          10
#define CYCLES_PER_FRAME 10

static void render_pixels(SDL_Renderer *renderer, const uint8_t *display) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            int index = y * DISPLAY_WIDTH + x;
            if (display[index]) {
                SDL_FRect r;
                r.x = x * SCALE;
                r.y = y * SCALE;
                r.w = SCALE;
                r.h = SCALE;
                SDL_RenderRect(renderer, &r);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char **argv) {
    chip8 chip;
    chip8_init(&chip);

    if (!chip8_load_ROM(&chip, "../src/IBM_Logo.ch8")) {
        printf("Failed to load ROM\n");
        return 1;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "CHIP-8 (SDL3)",
        DISPLAY_WIDTH * SCALE,
        DISPLAY_HEIGHT * SCALE,
        0
    );
    if (!win) {
        printf("SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(win, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    SDL_Event e;

 while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = 0;
            }
        }

        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            chip8_step(&chip);
        }

        render_pixels(renderer, chip.display);

        SDL_Delay(1);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

