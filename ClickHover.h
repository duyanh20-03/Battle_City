#ifndef CLICKHOVER_H_INCLUDED
#define CLICKHOVER_H_INCLUDED


#pragma once
#include <SDL.h>

class ClickHover {
public:
    static bool isMouseHover(const SDL_Rect& rect, int mouseX, int mouseY);

    static bool isMouseClick(const SDL_Rect& rect, const SDL_Event& e);
};
#endif // CLICKHOVER_H_INCLUDED
