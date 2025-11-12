#ifndef EXPLODE__H_INCLUDED
#define EXPLODE__H_INCLUDED

#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include "Constant.h"
class Explode {
public:
    int x, y;
    float lifetime;
    float maxLifetime;
    SDL_Rect rect;
    bool active;
    SDL_Texture* imageExplode;

    Explode(int startX, int startY, SDL_Texture* sharedTexture);
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    ~Explode();
};
#endif
