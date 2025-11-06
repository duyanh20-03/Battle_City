#ifndef LEVELSSCREEN_H_INCLUDED
#define LEVELSSCREEN_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Button.h"

class LevelsScreen {
public:
    LevelsScreen(SDL_Renderer* renderer);
    ~LevelsScreen();

    void init();
    void handleEvent(SDL_Event* e);
    void render();
    void cleanup();

    int getSelectedLevel(); // trả về level chọn (1, 2, 3...)

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    std::vector<Button> levelButtons;
    int Level;
};

#endif // LEVELSSCREEN_H_INCLUDED
