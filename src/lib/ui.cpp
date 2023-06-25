#include "ui.h"

ui::ui(SDL_Renderer *r, Player *p, SDL_FRect *vp) : player(p), render(r), viewport(vp) {
    // Reload
    SDL_Surface *emotesSheet = IMG_Load(emotesPath.c_str());
    emotes = SDL_CreateTextureFromSurface(render, emotesSheet);
    SDL_FreeSurface(emotesSheet);

    SDL_Surface *backboardSheet = IMG_Load(backboardPath.c_str());
    backboard = SDL_CreateTextureFromSurface(render, backboardSheet);
    SDL_FreeSurface(backboardSheet);
    // Banana
    SDL_Surface *bananaSurface = IMG_Load(bananaPath.c_str());
    bananaT = SDL_CreateTextureFromSurface(render, bananaSurface);
    SDL_FreeSurface(bananaSurface);

    //Ammo
    SDL_Surface *ammoSurface = IMG_Load(ammoPath.c_str());
    ammoT = SDL_CreateTextureFromSurface(render, ammoSurface);
    SDL_FreeSurface(ammoSurface);

    //Blood Overlay when dead
    SDL_Surface *deadScreenSurface = IMG_Load(deadScreenPath.c_str());
    deadScreen = SDL_CreateTextureFromSurface(render, deadScreenSurface);
    SDL_FreeSurface(deadScreenSurface);

    for (int i = 3; i >= 0; i--) {
        SDL_Rect newRect = {3 * 16, i * 16, 16, 16};
        reloadFrames.push_back(newRect);
    }

    //UI Font
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
    }
    font = TTF_OpenFont(BasePath "asset/font/MonkeyIsland-1991-refined.ttf", 32);
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

    if(player->health == 0) {
        SDL_FRect dstRect = {
                0,
                0,
                viewport->w,
                viewport->h
        };
        SDL_Texture *dead = deadScreen;
        SDL_SetTextureAlphaMod(dead, 255);
        SDL_RenderCopyF(render, dead, nullptr, &dstRect);

        SDL_Texture* loss = getLossScreen();
        SDL_RenderCopy(render, loss, nullptr, &winLossScreen);
        SDL_DestroyTexture(loss);

        SDL_Texture *info = getInfoScreen();
        SDL_RenderCopy(render, info, nullptr, &infoScreen);
        SDL_DestroyTexture(info);

    }


    if(won) {
        SDL_Texture* win = getWinScreen();
        SDL_RenderCopy(render, win, nullptr, &winLossScreen);
        SDL_DestroyTexture(win);

        SDL_Texture *info = getInfoScreen();
        SDL_RenderCopy(render, info, nullptr, &infoScreen);
        SDL_DestroyTexture(info);
    }

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
    SDL_DestroyTexture(ammoT);

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
    // Draw Currency
    auto curr = getCurrency();
    SDL_RenderCopy(render, curr, nullptr, &currencyCount);
    SDL_DestroyTexture(curr);
}

SDL_Texture *ui::getAmmo() {
    std::string ammoText = ": " + std::to_string(player->gun->ammo);

    SDL_Color white = {255, 255, 255, 255}; // RGBA

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, ammoText.c_str(), white);
    if (textSurface == nullptr) {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        return nullptr;
    }

    int ammoW = 50, ammoH = 50;
    int totalW = ammoW +10+ textSurface->w;
    int totalH = std::max(ammoH, textSurface->h);

    SDL_Texture* finalTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, totalW, totalH);
    SDL_SetRenderTarget(render, finalTexture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_Rect ammoRect = {0, 0, ammoW, ammoH};
    SDL_Rect textRect = {ammoW+ 5, 5, textSurface->w, textSurface->h};

    SDL_RenderCopy(render, ammoT, NULL, &ammoRect);

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_RenderCopy(render, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    SDL_SetRenderTarget(render, nullptr);
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(textSurface);

    // Set dimensions of ammoCount
    ammoCount = {24, 74, totalW, totalH};

    // Return the final texture
    return finalTexture;
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

SDL_Texture *ui::getCurrency() {
    std::string currencyText = ": " + std::to_string(player->currency);

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, currencyText.c_str(), white);
    if (textSurface == nullptr) {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        return nullptr;
    }

    int bananaW = 50, bananaH = 50;
    int totalW = bananaW + 10 + textSurface->w;
    int totalH = std::max(bananaH, textSurface->h);

    SDL_Texture* finalTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, totalW, totalH);
    SDL_SetRenderTarget(render, finalTexture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_Rect bananaRect = {0, 0, bananaW, bananaH};
    SDL_Rect textRect = {bananaW + 5, 5, textSurface->w, textSurface->h};

    SDL_RenderCopy(render, bananaT, NULL, &bananaRect);

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_RenderCopy(render, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);

    SDL_SetRenderTarget(render, NULL);
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(textSurface);

    currencyCount = {24, 130, totalW, totalH};

    return finalTexture;
}

ui::~ui() {
    SDL_DestroyTexture(emotes);
    SDL_DestroyTexture(backboard);
    TTF_CloseFont(font);

    for (SDL_Texture* heart : hearts) {
        SDL_DestroyTexture(heart);
    }
}

SDL_Texture *ui::getLossScreen() {
    std::string winText = "DU BIST GESTORBEN";

    SDL_Color white = {255, 0, 0, 255};

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, winText.c_str(), white);

    if (textSurface == nullptr) {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        return nullptr;
    }

    winLossScreen = {400, 200, 600, 200};


    SDL_Texture* finalTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 200);
    SDL_SetRenderTarget(render, finalTexture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(render, textSurface);

    SDL_RenderCopy(render, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);

    SDL_SetRenderTarget(render, NULL);
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(textSurface);

    return finalTexture;
}

SDL_Texture *ui::getWinScreen() {
    std::string WinText = "DU HAST GEWONNEN";

    SDL_Color white = {0, 128, 255, 255};

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, WinText.c_str(), white);
    if (textSurface == nullptr) {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        return nullptr;
    }

    winLossScreen = {400, 200, 600, 200};

    SDL_Texture* finalTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 200);
    SDL_SetRenderTarget(render, finalTexture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_RenderCopy(render, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_SetRenderTarget(render, NULL);
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(textSurface);

    return finalTexture;
}

SDL_Texture *ui::getInfoScreen() {
    std::string infoText = "LEER FUER NEUSTART";

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *textSurface = TTF_RenderText_Blended(font, infoText.c_str(), white);

    if (textSurface == nullptr) {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        return nullptr;
    }

    infoScreen = {400, 500, 600, 200};


    SDL_Texture* finalTexture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 200);
    SDL_SetRenderTarget(render, finalTexture);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
    SDL_RenderClear(render);

    SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(render, textSurface);

    SDL_RenderCopy(render, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);

    SDL_SetRenderTarget(render, NULL);
    SDL_SetTextureBlendMode(finalTexture, SDL_BLENDMODE_BLEND);

    SDL_FreeSurface(textSurface);

    return finalTexture;
}
