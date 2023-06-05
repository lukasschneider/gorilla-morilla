#ifndef SDL_BASEGAME_ENEMY_H
#define SDL_BASEGAME_ENEMY_H

#include "../lib/global.h"
#include "Bullet.h"

class Enemy {
public:
    SDL_FRect body;
    float hp;
    float maxHp;
    bool movingRight = true;
    float speed = 300.0f;

    Enemy(float x, float y, float maxHp);

    void update(float dt);

    void respawn();

    void render(SDL_Renderer *renderer, const SDL_FRect &viewport);

    void coll(std::vector<Bullet *> &bullets);
private:
    std::vector<Bullet*> hitBullets;
};



#endif //SDL_BASEGAME_ENEMY_H
