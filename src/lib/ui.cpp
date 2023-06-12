#include "ui.h"

ui::ui(SDL_Renderer *r, Player *p, SDL_FRect *vp) : player(p), render(r), viewport(vp) {
    // Reload
    SDL_Surface *emotesSheet = IMG_Load(emotesPath.c_str());
    emotes = SDL_CreateTextureFromSurface(render, emotesSheet);
    SDL_FreeSurface(emotesSheet);

    SDL_Surface *backboardSheet = IMG_Load(backboardPath.c_str());
    backboard = SDL_CreateTextureFromSurface(render, backboardSheet);
    SDL_FreeSurface(backboardSheet);

    /*
    for (int i = 0; i < 4; ++i) {
        SDL_Rect newRect = {(i * 16) + 27 * 16, 16 * 16, 16, 16};
        reloadFrames.push_back(newRect);
    }
     */
    for (int i = 3; i >= 0; i--) {
        SDL_Rect newRect = {3 * 16, i * 16, 16, 16};
        reloadFrames.push_back(newRect);
    }

    //UI Font
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
    }
    font = TTF_OpenFont(BasePath "asset/font/Kenney_Blocks.ttf", 32);
    if (font == nullptr) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
    }
    // Heart UI
    SDL_Surface *empty_heart = IMG_Load(BasePath "asset/graphic/ui/ui_heart_empty.png");
    hearts.push_back(SDL_CreateTextureFromSurface(render, empty_heart));
    SDL_FreeSurface(empty_heart);

    SDL_Surface *half_heart = IMG_Load(BasePath "asset/graphic/ui/ui_heart_half.png");
    hearts.push_back(SDL_CreateTextureFromSurface(render, half_heart));
    SDL_FreeSurface(half_heart);

    SDL_Surface *full_heart = IMG_Load(BasePath "asset/graphic/ui/ui_heart_full.png");
    hearts.push_back(SDL_CreateTextureFromSurface(render, full_heart));
    SDL_FreeSurface(full_heart);

}

void ui::update() {
    if (player->gun->isReloading) {
        reloadIndex = getReloadIndex();
    }
}

int ui::getReloadIndex() const {
    float normalizedTime = player->gun->timeSinceLastReload / player->gun->reloadTime;
    int frameIndex = static_cast<int>(floor(normalizedTime * 3));
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
        SDL_RenderCopyF(render, emotes, &srcRect, &dstRect);
    }
    // Draw Ammo
    SDL_Texture *ammoT = getAmmo();
    SDL_RenderCopy(render, ammoT, nullptr, &ammoCount);

    //Draw Hearts
    std::vector<SDL_Texture *> hp = getHearts();
    for (int i = 0; i < 3; i++) {
        SDL_Rect newRect = {
                15 + i * 56,
                10,
                64,
                64
        };
        SDL_RenderCopy(render, hp[i], nullptr, &newRect);
    }
}

SDL_Texture *ui::getAmmo() {
    std::string ammoText = "Ammo: " + std::to_string(player->gun->ammo);
    SDL_Surface *surface = TTF_RenderText_Solid(font, ammoText.c_str(), {255, 255, 255});
    if (surface == nullptr) {
        printf("TTF_RenderText_Solid: %s\n", TTF_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
    if (texture == nullptr) {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
    }

    ammoCount = {24, 74, surface->w, surface->h};


    SDL_FreeSurface(surface);

    return texture;
}

std::vector<SDL_Texture *> ui::getHearts() {
    int hp = player->health;
    std::vector<SDL_Texture *> displayHearts(3);
    for (int i = 0; i < 3; ++i) {
        if (hp >= 2) {
            displayHearts[i] = hearts[2];
            hp -= 2;
        } else if (hp == 1) {
            displayHearts[i] = hearts[1];
            hp -= 1;
        } else {
            displayHearts[i] = hearts[0];
        }
    }
    return displayHearts;
}

ui::~ui() {
    SDL_DestroyTexture(emotes);
    SDL_DestroyTexture(backboard);
    TTF_CloseFont(font);

    for (SDL_Texture* heart : hearts) {
        SDL_DestroyTexture(heart);
    }
}

