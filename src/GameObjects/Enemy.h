#ifndef SDL_BASEGAME_ENEMY_H
#define SDL_BASEGAME_ENEMY_H

#include "../lib/global.h"

class Enemy {
public:
    SDL_FRect body;
    float hp;
    float maxHp;

    Enemy(float x, float y, float maxHp);

    void update();

    void respawn();

    void render(SDL_Renderer *renderer, const SDL_FRect &viewport);
};



#endif //SDL_BASEGAME_ENEMY_H
