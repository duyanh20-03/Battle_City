#include "pauseScreen.h"
#include "Constant.h"


pauseScreen::pauseScreen(SDL_Renderer* renderer) {
    this->renderer = renderer;
    this->pauseTexture = nullptr;
}

pauseScreen::~pauseScreen() {
    cleanup();
}

void pauseScreen::init() {
    // Load background image
    pauseTexture = IMG_LoadTexture(renderer, "Menu.png");

    if (!pauseTexture) {
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

    pauseButtons.clear();
    pauseButtons.push_back(Button(centerX, startY, buttonWidth, buttonHeight, "-----PAUSE-----"));
    pauseButtons.push_back(Button(centerX, startY + buttonHeight + spacing, buttonWidth, buttonHeight, "Resume"));
    pauseButtons.push_back(Button(centerX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight, "Menu"));
    pauseButtons.push_back(Button(centerX, startY + 3 * (buttonHeight + spacing), buttonWidth, buttonHeight, "Quit"));
}

void pauseScreen::cleanup() {
    if (pauseTexture) {
        SDL_DestroyTexture(pauseTexture);
        pauseTexture = nullptr;
    }
    for (auto& btn : pauseButtons) {
        btn.cleanup();
    }
}

void pauseScreen::render() {
    if (pauseTexture) {
        SDL_RenderCopy(renderer, pauseTexture, nullptr, nullptr);
    } else {
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
        SDL_RenderClear(renderer);
    }

    for (auto& btn : pauseButtons) btn.render(renderer);

    SDL_RenderPresent(renderer);
}

void pauseScreen::handleEvent(SDL_Event* e) {
    for (auto& btn : pauseButtons) btn.handleEvent(e);
}

std::string pauseScreen::getClick() {
    for (auto& btn : pauseButtons) {
            if (btn.isClick) {
                btn.isClick = false;
                return btn.func;
            }
    }
    return "";
}
