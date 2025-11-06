#ifndef ENEMYTANK_H_INCLUDED
#define ENEMYTANK_H_INCLUDED

#include<SDL.h>
#include<SDL_image.h>
#include<vector>
#include<iostream>
#include "Bullet.h"
#include "Wall.h"

class EnemyTank {
    public:
    int x,y,dirX,dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    SDL_Texture* imageEnemy;
    bool active;
    std::vector<Bullet>bullets;

    EnemyTank(int startX, int startY, SDL_Renderer* renderer);
    void init(SDL_Renderer* renderer);
    void move(float deltaTime, const std::vector<Wall> walls);
    void shoot();
    void updateBullet();
    void render(SDL_Renderer* renderer);

};

#endif // ENEMYTANK_H_INCLUDED
