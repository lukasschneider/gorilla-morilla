#ifndef GORILLAGAME_BASEENEMY_H
#define GORILLAGAME_BASEENEMY_H


#include "../../lib/global.h"
#include "../../lib/rh.h"
#include "../Weapons/Bullet.h"
#include "../Weapons/Gun.h"
#include "../Pickups.h"


class BaseEnemy {
public:
    SDL_FRect dRect;
    float hp;
    float maxHp;
    float speed = 500.0f;
    std::vector<Pickup*>* activePowerUps;
    Path path;
    Texture * enemyTexture;
    std::string enemyPath = BasePath "asset/graphic/enemy/green.png";
    bool isHit = false;
    float hitTime = 0.0f;
    float hitDuration = 0.2f;

    BaseEnemy(float x, float y, float maxHp,std::vector<Pickup*>* pickup);

    virtual bool PickupRate();

    virtual void die();

    virtual void update(float dt, Room &room);

    virtual void respawn();

    virtual void render(const SDL_FRect &viewport);

    [[maybe_unused]] virtual void coll(BulletRingBuffer& bullets);

    virtual void attack() = 0;

    [[nodiscard]] virtual bool inRadius() const = 0;

    virtual void getHit();

    //virtual bool lineOfSightCheck(const Player &player, const std::vector<std::vector<int>> &collisionLayer);
};

#endif //GORILLAGAME_BASEENEMY_H
