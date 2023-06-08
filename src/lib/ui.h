#ifndef GORILLAGAME_UI_H
#define GORILLAGAME_UI_H

#include "global.h"
#include "../GameObjects/Player.h"


class ui {
public:
    Player *player;
    SDL_Renderer *render;
    SDL_FRect *viewport;

    // used for the reload animation
    std::string buttonsPath = BasePath "asset/graphic/ui/tilemap_packed.png";
    SDL_Texture *buttons;
    std::vector<SDL_Rect> reloadFrames;
    int reloadIndex;

    ui(SDL_Renderer *r, Player *p, SDL_FRect *vp);

    void update();

    void drawUi();

    int getReloadIndex() const;
};


#endif //GORILLAGAME_UI_H
