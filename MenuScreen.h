#ifndef MENUSCREEN_H_INCLUDED
#define MENUSCREEN_H_INCLUDED


#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Button.h"

class MenuScreen {
private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    bool inLevelSelect = false;

public:
    std::vector<Button> mainButtons;
    std::vector<Button> levelButtons;
    MenuScreen(SDL_Renderer* renderer);
    ~MenuScreen();
    void handleEvent(SDL_Event* e);
    std::string getClick();
    void init();
    void initLevels();
    void cleanup();
    void render();

    bool isInLevelSelect() const {return inLevelSelect;}
    void SetInLevelSelect(bool state) {inLevelSelect = state;}
};
#endif // MENUSCREEN_H_INCLUDED
