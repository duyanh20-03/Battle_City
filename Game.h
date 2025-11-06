#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Wall.h"
#include "Player.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "MenuScreen.h"

class Game {
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    std::vector<Wall> walls;
    Player player;
    int enemyNumber = 10;
    int currentLevel = 1;
    std::vector<EnemyTank> enemies;

    public:
    Game();
    ~Game();

    void run();
    void render();
    void handleEvents(float deltaTime);
    void update(float deltaTime);
    void generateWalls();
    void spawnEnemies();
    bool isRunning() const { return running; }

};


#endif // GAME_H_INCLUDED
