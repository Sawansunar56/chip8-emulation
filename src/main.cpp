#include "CHIP8/cpu.h"
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>


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
int main(int argc, char *argv[]) {
    // scc(SDL_Init(SDL_INIT_VIDEO));
    //
    // SDL_Window *window = (SDL_Window *)scp(SDL_CreateWindow(
    //     "SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    //     SCREEN_WIDTH, SCREEN_HEIGHT, 0));
    //
    // SDL_Renderer *renderer =
    //     (SDL_Renderer *)scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED));
    //
    //
    // bool running = true;
    // while (running) {
    //     SDL_Event event;
    //     while (SDL_PollEvent(&event)) {
    //         switch (event.type) {
    //         case SDL_QUIT:
    //             running = false;
    //             break;
    //
    //         default:
    //             break;
    //         }
    //     }
    //
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    //     SDL_RenderClear(renderer);
    //
    //     SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //     SDL_Rect rect;
    //
    //     rect.x = 100;
    //     rect.y = 100;
    //     rect.w = 200;
    //     rect.h = 150;
    //
    //     SDL_RenderFillRect(renderer, &rect);
    //
    //     SDL_RenderPresent(renderer);
    // }
    chip8 cs;


    return 0;
}
