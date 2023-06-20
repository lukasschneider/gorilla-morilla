#ifndef SDL_BASEGAME_ENEMY_H
#define SDL_BASEGAME_ENEMY_H

#include "../lib/global.h"
#include "../lib/rh.h"
#include "Bullet.h"
#include "Gun.h"
#include "Pickups.h"

class Room;
class Pickup;

class Enemy {
public:
    SDL_FRect body;
    float hp;
    float maxHp;
    bool movingRight = true;
    float speed = 300.0f;
    float radius = 64.0f;
    std::vector<Pickup*>* activePowerUps;
    Path path;

    Enemy(float x, float y, float maxHp, std::vector<Pickup*>* pickup);

    bool spawnrate();

    Enemy* die();

    void update(float dt, Room &room);

    void respawn();

    void render(SDL_Renderer *renderer, const SDL_FRect &viewport);

    void coll(BulletRingBuffer& bullets);

    static void attack();

    ~Enemy();

    [[nodiscard]] bool inRadius() const;
};



#endif //SDL_BASEGAME_ENEMY_H
