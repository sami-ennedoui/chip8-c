#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_log.h>
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
static int map_key(SDL_Keycode key) {
	switch (key) {
    case SDL_SCANCODE_1: return 0x1;
    case SDL_SCANCODE_2: return 0x2;
    case SDL_SCANCODE_3: return 0x3;
    case SDL_SCANCODE_4: return 0xC;

    case SDL_SCANCODE_Q: return 0x4;
    case SDL_SCANCODE_W: return 0x5;
    case SDL_SCANCODE_E: return 0x6;
    case SDL_SCANCODE_R: return 0xD;

    case SDL_SCANCODE_A: return 0x7;
    case SDL_SCANCODE_S: return 0x8;
    case SDL_SCANCODE_D: return 0x9;
    case SDL_SCANCODE_F: return 0xE;

    case SDL_SCANCODE_Z: return 0xA;
    case SDL_SCANCODE_X: return 0x0;
    case SDL_SCANCODE_C: return 0xB;
    case SDL_SCANCODE_V: return 0xF;

    default: return -1;
    }
}
int main(int argc, char **argv) {
    chip8 chip;
    chip8_init(&chip);
    if (!chip8_load_ROM(&chip, "../src/keypad_test.ch8")) {
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
if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP) {
        int idx = map_key(e.key.scancode);
        if (idx >= 0 && idx < 16) {
            chip.keys[idx] = (e.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;
        }
    }
}
//to do : find a better approach to handle timing issues to fix games not working issues 
static uint32_t last_tick = 0;
uint32_t now = SDL_GetTicks();
if (now - last_tick >= 1000 / 60) {
    if (chip.delay_timer > 0) chip.delay_timer--;
    if (chip.sound_timer > 0) chip.sound_timer--;
    last_tick = now;
}
        for (int i = 0; i < CYCLES_PER_FRAME; i++) {
            chip8_step(&chip);
        }

        render_pixels(renderer, chip.display);

        SDL_Delay(10);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

