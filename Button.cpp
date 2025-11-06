#include "Button.h"
#include "ClickHover.h"
#include<SDL_ttf.h>
#include<iostream>

Button::Button() : texture(nullptr) {}

Button::Button(int x, int y, int w, int h, const std::string& text) {
    rect = {x, y, w, h};
    func = text;
    texture = nullptr;
    hover = false;
    isClick = false;
}

void Button::handleEvent(SDL_Event* e) {
    static SDL_Cursor* handCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    static SDL_Cursor* arrowCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    int mouseX, mouseY ;
    SDL_GetMouseState(&mouseX, &mouseY);
    bool wasHover = hover;
    hover = ClickHover::isMouseHover(rect, mouseX, mouseY);

    if (hover && !wasHover) {
        SDL_SetCursor(handCursor); // hiện bàn tay khi hover
    } else if (!hover && wasHover) {
        SDL_SetCursor(arrowCursor); // trở lại mũi tên khi rời
    }

    // xử lý click
    if (hover && ClickHover::isMouseClick(rect, *e)) {
        isClick = true;
        std::cout << "Clicked: " << func << std::endl;
    }

}

void Button::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    if (hover) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }

    //SDL_RenderFillRect(renderer, &rect);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderDrawRect(renderer, &rect);

    TTF_Font* font = TTF_OpenFont("SigmarOne-Regular.ttf", 24);
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, func.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textW, textH;
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        SDL_Rect textRect = {rect.x + (rect.w - textW) / 2, rect.y + (rect.h - textH) / 2, textW, textH};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        TTF_CloseFont(font);
    }



}

void Button::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
