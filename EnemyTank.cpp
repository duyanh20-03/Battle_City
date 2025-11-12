#include<SDL_image.h>
#include "EnemyTank.h"
#include "Bullet.h"
#include "Constant.h"
#include "Wall.h"
#include <algorithm>

EnemyTank::EnemyTank(int startX, int startY, SDL_Renderer* renderer) {
        moveDelay = 15;
        shootDelay = 5;
        x = startX;
        y = startY;
        rect = {x, y, TILE_SIZE - 10, TILE_SIZE - 10};
        dirX = 0;
        dirY = 1;
        active = true;
        imageEnemy = nullptr;
        init(renderer);
}

void EnemyTank::init(SDL_Renderer* renderer) {
    if (imageEnemy == nullptr) {
        imageEnemy = IMG_LoadTexture(renderer, "EnemyTank.png");
    }
}

void EnemyTank::move(float deltaTime, const std::vector<Wall> walls) {
    float speed = 100.0f; // pixels per second

    int newX = x + static_cast<int>(dirX * speed * deltaTime);
    int newY = y + static_cast<int>(dirY * speed * deltaTime);
    SDL_Rect newRect = {newX, newY, TILE_SIZE - 10, TILE_SIZE - 10};

    for (auto &wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect))
            return;
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }

    // Random thay đổi hướng nhỏ mỗi frame
    if (rand() % 100 < 2) { // 2% mỗi frame
        int r = rand() % 4;
        if (r == 0) { dirX = 0; dirY = -1; }
        else if (r == 1) { dirX = 0; dirY = 1; }
        else if (r == 2) { dirX = -1; dirY = 0; }
        else if (r == 3) { dirX = 1; dirY = 0; }
    }
}

void EnemyTank::shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 1;
    bullets.push_back(Bullet(x+TILE_SIZE/2 + 5, y+TILE_SIZE/2 + 5, this->dirX * 6, this->dirY * 6 ));
}
void EnemyTank::updateBullet() {
    for (auto& bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(),bullets.end(),[](Bullet &b){return !b.active;}),bullets.end());
}

void EnemyTank::render(SDL_Renderer* renderer) {
    //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //SDL_RenderFillRect(renderer, &rect);
    double angle = 0.0;
    if (dirX == 0 && dirY < 0) angle = 0.0;      // dirY = -5
    else if (dirX == 0 && dirY > 0) angle = 180.0;  // dirY = 5
    else if (dirX < 0 && dirY == 0) angle = 270.0;  // dirX = -5
    else if (dirX > 0 && dirY == 0) angle = 90.0;

    SDL_RenderCopyEx(renderer, imageEnemy, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    for(auto &bullet : bullets){
        bullet.render(renderer);
    }
}
