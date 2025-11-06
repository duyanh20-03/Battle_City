#include "MenuScreen.h"
#include "Constant.h"
MenuScreen::MenuScreen(SDL_Renderer* renderer) {
    this->renderer = renderer;
    this->backgroundTexture = nullptr;
}

MenuScreen::~MenuScreen() {
    cleanup();
}

void MenuScreen::init() {
    // Load background image
    backgroundTexture = IMG_LoadTexture(renderer, "Menu.png");

    if (!backgroundTexture) {
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

    mainButtons.clear();
    mainButtons.push_back(Button(centerX, startY, buttonWidth, buttonHeight, "Play"));
    mainButtons.push_back(Button(centerX, startY + buttonHeight + spacing, buttonWidth, buttonHeight, "Levels"));
    mainButtons.push_back(Button(centerX, startY + 2 * (buttonHeight + spacing), buttonWidth, buttonHeight, "Quit"));
}

void MenuScreen::initLevels() {
    inLevelSelect = true;
    levelButtons.clear();

    int buttonWidth = 200;
    int buttonHeight = 50;
    int spacingX = 60; // khoảng cách giữa 2 cột
    int spacingY = 20; // khoảng cách giữa các hàng

    // Tính vị trí căn giữa 2 cột
    int totalWidth = buttonWidth * 2 + spacingX;
    int startX = (SCREEN_WIDTH - totalWidth) / 2;
    int startY = (SCREEN_HEIGHT - (3 * buttonHeight + 2 * spacingY)) / 2 + 60;

    // 2 cột x 3 hàng cho 6 level
    // Cột trái
    levelButtons.push_back(Button(startX, startY, buttonWidth, buttonHeight, "Level 1"));
    levelButtons.push_back(Button(startX, startY + (buttonHeight + spacingY), buttonWidth, buttonHeight, "Level 3"));
    levelButtons.push_back(Button(startX, startY + 2 * (buttonHeight + spacingY), buttonWidth, buttonHeight, "Level 5"));

    // Cột phải
    levelButtons.push_back(Button(startX + buttonWidth + spacingX, startY, buttonWidth, buttonHeight, "Level 2"));
    levelButtons.push_back(Button(startX + buttonWidth + spacingX, startY + (buttonHeight + spacingY), buttonWidth, buttonHeight, "Level 4"));
    levelButtons.push_back(Button(startX + buttonWidth + spacingX, startY + 2 * (buttonHeight + spacingY), buttonWidth, buttonHeight, "Level 6"));

    // Nút Back (ở giữa, bên dưới)
    int backX = (SCREEN_WIDTH - buttonWidth) / 2;
    int backY = startY + 3 * (buttonHeight + spacingY) + 20;
    levelButtons.push_back(Button(backX, backY, buttonWidth, buttonHeight, "Back"));
}


void MenuScreen::cleanup() {
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
    for (auto& btn : mainButtons) {
        btn.cleanup();
    }
    for (auto& btn : levelButtons) {
        btn.cleanup();
    }
}

void MenuScreen::render() {
    if (backgroundTexture) {
        // Render background image - full screen
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    } else {
        // Fallback: Render solid color if image not found
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
        SDL_RenderClear(renderer);
    }

    if (!inLevelSelect) {
        for (auto& btn : mainButtons) btn.render(renderer);
    } else {
        for (auto& btn : levelButtons) btn.render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void MenuScreen::handleEvent(SDL_Event* e) {
    if (!inLevelSelect) {
        for (auto& btn : mainButtons) btn.handleEvent(e);
    } else {
        for (auto& btn : levelButtons) btn.handleEvent(e);
    }
}

std::string MenuScreen::getClick() {
    if (!inLevelSelect) {
        for (auto& btn : mainButtons) {
            if (btn.isClick) {
                btn.isClick = false;
                return btn.func;
            }
        }
    } else {
        for (auto& btn : levelButtons) {
            if (btn.isClick) {
                btn.isClick = false;
                return btn.func;
            }
        }
    }
    return "";
}
