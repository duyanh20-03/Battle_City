#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Wall.h"

class Game {
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    std::vector<Wall> walls;

    public:
    Game();
    ~Game();

    void run();
    void render();
    void generateWalls();
    bool isRunning() const { return running; }

};


#endif // GAME_H_INCLUDED
