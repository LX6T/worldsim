//
// Created by alux on 2/19/25.
//

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_TITLE "01 Open Window"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void game_cleanup(struct Game *game);
bool sdl_initialise(struct Game *game);

int
main(char argc, char *argv[])
{
    struct Game game = {
        .window = NULL,
        .renderer = NULL
    };

    if (sdl_initialise(&game)) {
        game_cleanup(&game);
        printf("All bad!\n");
        exit(1);
    }

    SDL_RenderClear(game.renderer);

    SDL_RenderPresent(game.renderer);

    SDL_Delay(5000);

    printf("All good!\n");

    game_cleanup(&game);
    return 0;
}

void
game_cleanup(struct Game *game)
{
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

bool
sdl_initialise(struct Game *game)
{
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "Error initialising SDL: %s\n", SDL_GetError());
        return true;
    }

    game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!game->window) {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return true;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (!game->renderer) {
        fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
        return true;
    }

    return false;
}