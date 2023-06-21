#ifndef SDL_BASEGAME_ENEMY_H
#define SDL_BASEGAME_ENEMY_H

#include "../lib/global.h"
#include "../lib/rh.h"
#include "Weapons/Bullet.h"
#include "Weapons/Gun.h"
#include "Pickups.h"
#include "Weapons/Knife.h"

class Room;
class Pickup;
class Knife;


class Enemy {
public:
    std::string enemyPath = BasePath "asset/graphic/enemy/green.png";
    SDL_FRect dRect;
    Texture * enemyTexture;
    std::unique_ptr<Knife> knife;
    float hp;
    float maxHp;
    bool movingRight = true;
    float speed = 300.0f;
    float radius = 64.0f;
    std::vector<Pickup*>* activePowerUps;
    Path path;
    bool isHit = false;
    float hitTime = 0.0f;
    float hitDuration = 0.2f;

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

    void getHit();

    void attackUpdate();
};



#endif //SDL_BASEGAME_ENEMY_H
