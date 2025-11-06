#include "ClickHover.h"

bool ClickHover::isMouseHover(const SDL_Rect& rect, int mouseX, int mouseY) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

bool ClickHover::isMouseClick(const SDL_Rect& rect, const SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int x = e.button.x;
        int y = e.button.y;
        return isMouseHover(rect, x, y);
    }
    return false;
}
