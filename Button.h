#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include<SDL.h>
#include<string>

class Button {
public:
    SDL_Rect rect;
    SDL_Texture* texture;
    std::string func;
    bool hover = false;
    bool isClick;

    Button();
    Button(int x, int y, int w, int h, const std::string& text);
    void handleEvent(SDL_Event*e);
    void render(SDL_Renderer* renderer);
    void cleanup();
};

#endif // BUTTON_H_INCLUDED
