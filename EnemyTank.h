#ifndef ENEMYTANK_H_INCLUDED
#define ENEMYTANK_H_INCLUDED

#include<SDL.h>
#include<vector>
#include<iostream>
#include "Bullet.h"
#include "Wall.h"

class EnemyTank {
    public:
    int x,y,dirX,dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    std::vector<Bullet>bullets;

    EnemyTank(int startX, int startY);
    void move(const std::vector<Wall> walls);
    void shoot();
    void updateBullet();
    void render(SDL_Renderer* renderer);

};

#endif // ENEMYTANK_H_INCLUDED
