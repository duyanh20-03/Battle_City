#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include<SDL.h>
#include<SDL_image.h>
#include "Wall.h"
#include "Bullet.h"
#include<iostream>
#include<vector>

class Player {
public:
    int x, y, dirX, dirY;
    SDL_Rect rect;
    SDL_Texture* imagePlayer1;
    std::vector<Bullet> bullets;
    int lives = 3;

    Player();
    Player(int startX, int startY, SDL_Renderer* renderer);
    void init(SDL_Renderer* renderer);
    //void cleanup();
    void move(int dx, int dy, float deltaTime, const std::vector<Wall>& walls);
    void render(SDL_Renderer* renderer);
    void shoot();
    void updateBullets();
    ~Player();


};

#endif // PLAYER_H_INCLUDED
