#ifndef GORILLAGAME_KNIFE_H
#define GORILLAGAME_KNIFE_H

#include "../../lib/global.h"
#include "../../lib/rh.h"
#include "../../lib/ph.h"

class Player;

class Knife {
public:
    float angle = 0.0f;
    const float stabTime = 0.2f;
    float timeSinceLastLondon = 0.0f;

    std::string path = BasePath "asset/graphic/guns/bigKnife.png";
    SDL_Texture* texture;
    SDL_Rect srcRect{};
    SDL_FRect dstRect{};

    bool isSlashing = false; // indicates if knife is slashing
    float slashingStartTime = 0.0f; // stores the start time of slash

    Knife();

    void renderKnife(const SDL_FRect &viewport);
    void updateKnife(const SDL_FRect &enemyRect, float dt);
    void slash();
};


#endif //GORILLAGAME_KNIFE_H
