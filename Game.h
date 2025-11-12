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
#include "Win.h"
#include "Explode.h"
#include "SoundManager.h"

class Game {
    private:
    SoundManager soundManager;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* hearts[3];
    SDL_Texture* explodeTexture = nullptr;
    //Mix_Music* menuMusic = nullptr;
    bool running;
    std::vector<Wall> walls;
    std::vector<Explode> explosions;
    Player player;
    int enemyNumber = 10;
    int currentLevel = 1;
    int lives = 3;
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
