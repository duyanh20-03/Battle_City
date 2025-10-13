#include "Wall.h"
#include "Constant.h"
#include<SDL_image.h>

 Wall::Wall(int startX, int startY) {
            x=startX;
            y=startY;
            active = true;
            rect={x,y,TILE_SIZE,TILE_SIZE};
            imageTextureWall = nullptr;

        }

void Wall::render(SDL_Renderer* renderer) {
    if(active){
              //  SDL_SetRenderDrawColor(renderer,150,75,0,255);
               // SDL_RenderFillRect(renderer,&rect);
                SDL_Texture* imageTextureWall = IMG_LoadTexture(renderer,"Walls.png");
                SDL_RenderCopy(renderer,imageTextureWall,nullptr,&rect);
            }
}

Wall::~Wall() {
    if (imageTextureWall) {
        SDL_DestroyTexture(imageTextureWall);
        imageTextureWall = nullptr;
    }
}

//void Wall::render() {}
