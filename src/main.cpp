#include "CHIP8/cpu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <chrono>
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

/// Checks if SDL result is a negative number and then it produces an error.
void scc(int code) {
    if (code < 0) {
        fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
}

/// Takes a void pointer and if it is null exits else returns the pointer.
void *scp(void *ptr) {
    if (ptr == NULL) {
        fprintf(stderr, "SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}

bool processInput(unsigned char *keys) {
    bool quit = false;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            quit = true;
        } break;

        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: {
                quit = true;
            } break;

            case SDLK_x: {
                keys[0] = 1;
            } break;

            case SDLK_1: {
                keys[1] = 1;
            } break;

            case SDLK_2: {
                keys[2] = 1;
            } break;

            case SDLK_3: {
                keys[3] = 1;
            } break;

            case SDLK_q: {
                keys[4] = 1;
            } break;

            case SDLK_w: {
                keys[5] = 1;
            } break;

            case SDLK_e: {
                keys[6] = 1;
            } break;

            case SDLK_a: {
                keys[7] = 1;
            } break;

            case SDLK_s: {
                keys[8] = 1;
            } break;

            case SDLK_d: {
                keys[9] = 1;
            } break;

            case SDLK_z: {
                keys[0xA] = 1;
            } break;

            case SDLK_c: {
                keys[0xB] = 1;
            } break;

            case SDLK_4: {
                keys[0xC] = 1;
            } break;

            case SDLK_r: {
                keys[0xD] = 1;
            } break;

            case SDLK_f: {
                keys[0xE] = 1;
            } break;

            case SDLK_v: {
                keys[0xF] = 1;
            } break;
            }
        } break;

        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case SDLK_x: {
                keys[0] = 0;
            } break;

            case SDLK_1: {
                keys[1] = 0;
            } break;

            case SDLK_2: {
                keys[2] = 0;
            } break;

            case SDLK_3: {
                keys[3] = 0;
            } break;

            case SDLK_q: {
                keys[4] = 0;
            } break;

            case SDLK_w: {
                keys[5] = 0;
            } break;

            case SDLK_e: {
                keys[6] = 0;
            } break;

            case SDLK_a: {
                keys[7] = 0;
            } break;

            case SDLK_s: {
                keys[8] = 0;
            } break;

            case SDLK_d: {
                keys[9] = 0;
            } break;

            case SDLK_z: {
                keys[0xA] = 0;
            } break;

            case SDLK_c: {
                keys[0xB] = 0;
            } break;

            case SDLK_4: {
                keys[0xC] = 0;
            } break;

            case SDLK_r: {
                keys[0xD] = 0;
            } break;

            case SDLK_f: {
                keys[0xE] = 0;
            } break;

            case SDLK_v: {
                keys[0xF] = 0;
            } break;
            }
        } break;
        }
    }

    return quit;
}

int main(int argc, char *argv[]) {
    scc(SDL_Init(SDL_INIT_VIDEO));
    
    int width = 64, height = 32;
    SDL_Window *window = (SDL_Window *)scp(SDL_CreateWindow(
        "SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width * 10, height * 10, SDL_WINDOW_SHOWN));


    SDL_Renderer *renderer = (SDL_Renderer *)scp(
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));

    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STREAMING, width, height);
    chip8 cs;
    int videoPitch = sizeof(cs.display[0]) * 64;
    bool running = false;
    int cycleDelay = 1;

    cs.LoadROM("Space Invaders [David Winter].ch8");

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
    while (!running) {
        running = processInput(cs.key);
        auto currentTime = std::chrono::high_resolution_clock::now();
        float dt =
            std::chrono::duration<float, std::chrono::milliseconds::period>(
                currentTime - lastCycleTime)
                .count();

        if (dt > cycleDelay) {
            lastCycleTime = currentTime;
            cs.emulateCycle();
            
            SDL_UpdateTexture(texture, NULL, cs.display,
                              videoPitch);

            // Render the updated texture
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    return 0;
}
