#include "Player.h"
#include "Bullet.h"
#include "Wall.h"
#include "Constant.h"
#include <algorithm>


Player::Player() {
    x = 0;
    y = 0;
    rect = {x, y, TILE_SIZE - 10, TILE_SIZE - 10};
    dirX = 0;
    dirY = -1;
    imagePlayer1 = nullptr;
}

Player::Player(int startX, int startY,SDL_Renderer* renderer) {
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE - 10, TILE_SIZE - 10};
    dirX = 0;
    dirY = -1;
    imagePlayer1 = nullptr;
    init(renderer);
}

void Player::init(SDL_Renderer* renderer) {
    if (imagePlayer1 == nullptr) {
        imagePlayer1 = IMG_LoadTexture(renderer, "PlayerTank.png");
    }
}

//void Player::cleanup() {
  //  if (imagePlayer1 != nullptr) {
    //    SDL_DestroyTexture(imagePlayer1);
      //  imagePlayer1 = nullptr;
    //}
//}

void Player::move(int dx, int dy, float deltaTime, const std::vector<Wall>& walls) {
    int moveX = static_cast<int>(dx * deltaTime);
    int moveY = static_cast<int>(dy * deltaTime);
    int newX = x + moveX;
    int newY = y + moveY;
    this->dirX = dx;
    this->dirY = dy;
    SDL_Rect newRect={newX,newY,TILE_SIZE-10,TILE_SIZE-10};
    for (auto &wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect,&wall.rect)) {
            return;
        }
    }

    if (newX > TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2
        && newY > TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
            x = newX;
            y = newY;
            rect.x = x;
            rect.y = y;
        }
}

void Player::render(SDL_Renderer* renderer){
    double angle = 0.0;
    if (dirX == 0 && dirY == -1) angle = 0.0;
    else if (dirX == 0 && dirY == 1) angle = 180.0;
    else if (dirX == -1 && dirY == 0) angle = 270.0;
    else if (dirX == 1 && dirY == 0) angle = 90.0;

    SDL_RenderCopyEx(renderer, imagePlayer1, nullptr, &rect, angle, nullptr, SDL_FLIP_NONE);
    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }

}

void Player::shoot() {
    std::cout << "Shoot! bullets size before: " << bullets.size() << std::endl;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5, y + TILE_SIZE / 2 - 5, this->dirX * 8, this->dirY * 8));
    std::cout << "After: " << bullets.size() << std::endl;
}

void Player::updateBullets() {
    for (auto& bullet : bullets) {
        bullet.move();
    }
    bullets.erase(std::remove_if(bullets.begin(),bullets.end(),[](Bullet &b){return !b.active;}),bullets.end());


}

Player::~Player() {
    //cleanup();
}
