#ifndef PAUSESCREEN_H_INCLUDED
#define PAUSESCREEN_H_INCLUDED

#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Button.h"

class pauseScreen {
private:
    SDL_Renderer* renderer;
    SDL_Texture* pauseTexture;

public:
    std::vector<Button> pauseButtons;
    pauseScreen(SDL_Renderer* renderer);
    ~pauseScreen();
    void handleEvent(SDL_Event* e);
    std::string getClick();
    void init();
    void cleanup();
    void render();
};

#endif // PAUSESCREEN_H_INCLUDED
