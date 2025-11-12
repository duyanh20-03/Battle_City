#include "Wall.h"
#include "Constant.h"
#include<SDL_image.h>

 Wall::Wall(int startX, int startY) {
            x = startX;
            y = startY;
            active = true;
            rect = {x, y, TILE_SIZE - 10, TILE_SIZE - 10};
            imageTextureWall = nullptr;
}


void Wall::render(SDL_Renderer* renderer) {
    imageTextureWall = IMG_LoadTexture(renderer, "Walls.png");
    if (active && imageTextureWall) {
        SDL_RenderCopy(renderer, imageTextureWall, nullptr, &rect);
    }
}

Wall::~Wall() {
    if (imageTextureWall) {
        SDL_DestroyTexture(imageTextureWall);
        imageTextureWall = nullptr;
    }
}

//void Wall::render() {}
