#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_TITLE "Random Walk"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void game_cleanup(struct Game *game, int exit_status);
bool sdl_initialize(struct Game *game);

int main() {
    struct Game game = {
        .window = NULL,
        .renderer = NULL,
    };

    if (sdl_initialize(&game)) {
        game_cleanup(&game, EXIT_FAILURE);
    }

    srand(time(NULL));

    int x = 25;
    int y = 25;

    while (true) {

        uint8_t dir = rand() % 4;

        switch (dir)
        {
            case 0:
                x -= 1;
                y -= 1;
                break;
            case 1:
                x += 1;
                y -= 1;
                break;
            case 2:
                x += 1;
                y += 1;
                break;
            case 3:
                x -= 1;
                y += 1;
                break;
        }

        if (x > 50 || x < 0 || y > 50 || y < 0)
        {
            x = (x + 50) % 50;
            y = (y + 50) % 50;
        }
        
        SDL_SetRenderDrawColor(game.renderer,
            (dir == 0 || dir == 1) ? 255 : 0,
            (dir == 1 || dir == 2) ? 255 : 0,
            (dir == 3) ? 255 : 0,
            255);
        SDL_RenderDrawPoint(game.renderer, x, y);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                game_cleanup(&game, EXIT_SUCCESS);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    game_cleanup(&game, EXIT_SUCCESS);
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        SDL_RenderPresent(game.renderer);

        SDL_Delay(4);
    }

    game_cleanup(&game, EXIT_SUCCESS);

    return 0;
}

void game_cleanup(struct Game *game, int exit_status) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    IMG_Quit();
    SDL_Quit();
    exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return true;
    }

    game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                    SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return true;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return true;
    }
    SDL_RenderSetScale(game->renderer, 20, 20);

    srand((unsigned)time(NULL));

    return false;
}