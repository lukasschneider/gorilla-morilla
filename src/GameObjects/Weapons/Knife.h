#ifndef GORILLAGAME_KNIFE_H
#define GORILLAGAME_KNIFE_H

#include "../../lib/global.h"
#include "../../lib/rh.h"
#include "../../lib/ph.h"

class Player;

class Knife {
public:
    float angle = 0.0f;
    [[maybe_unused]] const float stabTime = 0.2f;
    [[maybe_unused]] float timeSinceLastLondon = 0.0f;

    std::string path = BasePath "asset/graphic/guns/bigKnife.png";
    SDL_Texture* texture;
    [[maybe_unused]] SDL_Rect srcRect{};
    SDL_FRect dstRect{};

    [[maybe_unused]] bool isSlashing = false;
    [[maybe_unused]] float slashingStartTime = 0.0f;

    Knife();

    void renderKnife(const SDL_FRect &viewport) const;
    void updateKnife(const SDL_FRect &enemyRect, [[maybe_unused]] float dt);

};


#endif //GORILLAGAME_KNIFE_H
