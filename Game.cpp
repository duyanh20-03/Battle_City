#include "Game.h"
#include "Constant.h"

Game::Game() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << "\n";
        running = false;
        return;
    }

    window = SDL_CreateWindow("Battle City",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        running = false;
        return;
    }

    running = true;
    generateWalls();
}

void Game::generateWalls() {
    for (int i = 3; i < MAP_HEIGHT - 3; i += 2) {
        for (int j = 3; j < MAP_WIDTH - 3; j += 2) {
            Wall w = Wall(j * TILE_SIZE, i * TILE_SIZE);
            walls.push_back(w);
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 125,125,125,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    for(int i=1;i<MAP_HEIGHT-1;i++){
                for(int j=1;j<MAP_WIDTH-1;j++){
                    SDL_Rect tile = {j*TILE_SIZE,i*TILE_SIZE,TILE_SIZE,TILE_SIZE};
                    SDL_RenderFillRect(renderer,&tile);
                }
            }
    for (auto& wall : walls) {
        wall.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        render();
        SDL_Delay(16); // ~60fps
    }
}

Game::~Game() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
