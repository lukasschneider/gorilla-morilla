#include "ui.h"

ui::ui(SDL_Renderer *r, Player *p, SDL_FRect *vp) : player(p), render(r), viewport(vp) {
    // Reload
    SDL_Surface *buttonsSpritesheet = IMG_Load(buttonsPath.c_str());
    buttons = SDL_CreateTextureFromSurface(render, buttonsSpritesheet);
    SDL_FreeSurface(buttonsSpritesheet);

    for (int i = 0; i < 4; ++i) {
        SDL_Rect newRect = {(i * 16) + 27 * 16, 16 * 16, 16, 16};
        reloadFrames.push_back(newRect);
    }

    //UI Font
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
    }
    font = TTF_OpenFont(BasePath "asset/font/Kenney_Blocks.ttf",24);
    if (font == nullptr) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
}

void ui::update() {
    if (player->gun->isReloading) {
        reloadIndex = getReloadIndex();
    }
}

int ui::getReloadIndex() const {
    float normalizedTime = player->gun->timeSinceLastReload / player->gun->reloadTime;
    int frameIndex = static_cast<int>(floor(normalizedTime * 4));
    return frameIndex;
}

void ui::drawUi() {
    if (player->gun->isReloading) {
        SDL_Rect srcRect = reloadFrames[reloadIndex];
        SDL_FRect dstRect = {
                player->dRect.x + player->dRect.w / 2 - viewport->x - 16,
                player->dRect.y - player->dRect.h / 2 - viewport->y,
                32,
                32
        };
        SDL_RenderCopyF(render, buttons, &srcRect, &dstRect);
    }
    // Draw Ammo
    SDL_Texture * ammoT = getAmmo();
    SDL_RenderCopy(render,ammoT, nullptr,&ammoCount);
}

SDL_Texture* ui::getAmmo() {
    std::string ammoText = "Ammo: " + std::to_string(player->gun->ammo);
    SDL_Surface* surface = TTF_RenderText_Solid(font, ammoText.c_str(), {255,255,255});
    if (surface == nullptr) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    if (texture == nullptr) {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    ammoCount = {15, 10, surface->w, surface->h};


    SDL_FreeSurface(surface);

    return texture;
}

