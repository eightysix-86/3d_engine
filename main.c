#include <SDL.h>
#include <stdio.h>

#include "math/matrix.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *win = SDL_CreateWindow("3D Engine", 100, 100, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int rect_width = 200, rect_height = 150;
    int rect_x = WIN_WIDTH / 2 - rect_width / 2, rect_y = WIN_HEIGHT / 2 - rect_height / 2;
    SDL_Rect rect = {rect_x, rect_y, rect_width, rect_height};
    size_t d = 10;

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:    rect_y -= d; break;
                    case SDLK_DOWN:  rect_y += d; break;
                    case SDLK_LEFT:  rect_x -= d; break;
                    case SDLK_RIGHT: rect_x += d; break;
                }
            }
        }
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        // Drawing
        rect.x = rect_x;
        rect.y = rect_y;
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255); // Red
        SDL_RenderDrawRect(ren, &rect); 
        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
