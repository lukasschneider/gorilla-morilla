#ifndef SDL_BASEGAME_ENEMY_H
#define SDL_BASEGAME_ENEMY_H

#include "../lib/global.h"
#include "../lib/rh.h"
#include "Bullet.h"
#include "Gun.h"
#include "Pickups.h"


class Enemy {
public:
    SDL_FRect body;
    float hp;
    float maxHp;
    bool movingRight = true;
    float speed = 300.0f;
    std::vector<Pickup*>* activePowerUps;

    Enemy(float x, float y, float maxHp,std::vector<Pickup*>* pickup);

    bool spawnrate();

    void die();

    void update(float dt);

    void respawn();

    void render(SDL_Renderer *renderer, const SDL_FRect &viewport);

    void coll(BulletRingBuffer& bullets);
};



#endif //SDL_BASEGAME_ENEMY_H
