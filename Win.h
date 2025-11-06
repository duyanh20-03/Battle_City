#ifndef WIN_H_INCLUDED
#define WIN_H_INCLUDED

#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Button.h"

class Win {
private:
    SDL_Renderer* renderer;
    SDL_Texture* winTexture;

public:
    std::vector<Button> winButtons;
    Win(SDL_Renderer* renderer);
    ~Win();
    void handleEvent(SDL_Event* e);
    std::string getClick();
    void init();
    void cleanup();
    void render();
};

#endif // WIN_H_INCLUDED
