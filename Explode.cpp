#include "Explode.h"

 Explode::Explode(int startX, int startY, SDL_Texture* sharedTexture) {
            x = startX;
            y = startY;
            active = true;
            rect = {x, y, TILE_SIZE - 10, TILE_SIZE - 10};
            imageExplode = sharedTexture;
            lifetime = 0.0f;
            maxLifetime = 0.3f;
}
void Explode::update(float deltaTime) {
    if (active) {
        lifetime += deltaTime;
        if (lifetime >= maxLifetime) {
            active = false;
        }
    }
}

void Explode::render(SDL_Renderer* renderer) {
    if (active) {
        SDL_RenderCopy(renderer, imageExplode, nullptr, &rect);
    }
}

Explode::~Explode() {
    imageExplode = nullptr;
}
