#include "ui.h"

ui::ui(SDL_Renderer *r, Player *p, SDL_FRect *vp) : player(p), render(r), viewport(vp) {
    SDL_Surface *buttonsSpritesheet = IMG_Load(buttonsPath.c_str());
    buttons = SDL_CreateTextureFromSurface(render, buttonsSpritesheet);
    SDL_FreeSurface(buttonsSpritesheet);

    for (int i = 0; i < 4; ++i) {
        SDL_Rect newRect = {(i * 16) + 27 * 16, 16 * 16, 16, 16};
        reloadFrames.push_back(newRect);
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
}

