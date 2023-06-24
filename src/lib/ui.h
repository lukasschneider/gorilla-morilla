#ifndef GORILLAGAME_UI_H
#define GORILLAGAME_UI_H

#include "global.h"
#include "../GameObjects/Player.h"


class ui {
public:
    Player *player;
    SDL_Renderer *render;
    SDL_FRect *viewport;

    TTF_Font* font;
    SDL_Rect ammoCount;
    SDL_Rect currencyCount;
    SDL_Rect winLossScreen;
    bool won = false;

    //stores heart texutres
    std::vector<SDL_Texture*> hearts;

    // used for the reload animation
    std::string buttonsPath = BasePath "asset/graphic/ui/tilemap_packed.png";
    SDL_Texture *buttons;
    // other reload spritesheet
    std::string emotesPath = BasePath "asset/graphic/ui/emotes.png";
    SDL_Texture *emotes;
    std::vector<SDL_Rect> reloadFrames;
    int reloadIndex;
    // ui backboard
    std::string backboardPath = BasePath "asset/graphic/ui/UIpackSheet_transparent.png";
    SDL_Texture * backboard;

    std::string bananaPath = BasePath "asset/graphic/pickups/banana.png";
    SDL_Texture * bananaT;

    std::string ammoPath = BasePath "asset/graphic/pickups/ammo.png";
    SDL_Texture * ammoT;

    ui(SDL_Renderer *r, Player *p, SDL_FRect *vp);

    void update();

    void drawUi();

    [[nodiscard]] int getReloadIndex() const;

    SDL_Texture* getAmmo();

    SDL_Texture *getCurrency();

    std::vector<SDL_Texture*> getHearts();

    ~ui();

private:

    SDL_Texture* getWinScreen();

    SDL_Texture* getLossScreen();


};

#endif //GORILLAGAME_UI_H
