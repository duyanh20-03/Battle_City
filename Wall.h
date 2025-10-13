#ifndef WALL__H_INCLUDED
#define WALL__H_INCLUDED

#include<SDL.h>
#include<iostream>
#include<vector>

class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;
    SDL_Texture* imageTextureWall;

    Wall(int startX, int startY);
    void render(SDL_Renderer* renderer);
    ~Wall();
};


#endif // WALL__H_INCLUDED
