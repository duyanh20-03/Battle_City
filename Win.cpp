#include "Win.h"
#include "Constant.h"

Win::Win(SDL_Renderer* renderer) {
    this->renderer = renderer;
    this->winTexture = nullptr;
}

Win::~Win() {
    cleanup();
}

void Win::init() {
    // Load background image
    winTexture = IMG_LoadTexture(renderer, "YouWin.png");

    if (!winTexture) {
        std::cout << "Warning: Cannot load menu_background.png - " << IMG_GetError() << std::endl;
        std::cout << "Menu will use solid color instead." << std::endl;
    } else {
        std::cout << "Successfully loaded menu_background.png" << std::endl;
    }

    int buttonWidth = 200;
    int buttonHeight = 50;
    int spacing = 20; // khoảng cách giữa các nút

    int startY = (SCREEN_HEIGHT - (3 * buttonHeight + 2 * spacing)) / 2 + 100; // căn giữa dọc
    int centerX = (SCREEN_WIDTH - buttonWidth) / 2; // căn giữa ngang

    winButtons.clear();
    winButtons.push_back(Button(centerX, startY, buttonWidth, buttonHeight, "Menu"));
    winButtons.push_back(Button(centerX, startY + buttonHeight + spacing, buttonWidth, buttonHeight, "Levels"));
    winButtons.push_back(Button(centerX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight, "Quit"));
}

void Win::cleanup() {
    if (winTexture) {
        SDL_DestroyTexture(winTexture);
        winTexture = nullptr;
    }
    for (auto& btn : winButtons) {
        btn.cleanup();
    }
}

void Win::render() {
    if (winTexture) {
        SDL_RenderCopy(renderer, winTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
        SDL_RenderClear(renderer);
    }

    for (auto& btn : winButtons) btn.render(renderer);

    SDL_RenderPresent(renderer);
}

void Win::handleEvent(SDL_Event* e) {
    for (auto& btn : winButtons) btn.handleEvent(e);
}

std::string Win::getClick() {
    for (auto& btn : winButtons) {
            if (btn.isClick) {
                btn.isClick = false;
                return btn.func;
            }
    }
    return "";
}
