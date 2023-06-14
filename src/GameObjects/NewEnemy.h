#ifndef SDL_BASEGAME_ENEMY_H
#define SDL_BASEGAME_ENEMY_H

#include "../lib/global.h"
#include "../Map/Room.h"
#include "Player.h"

class Room;
class Player;

enum EnemyState{
    Idle,
    Patrol,
    Attack,
    Damaged
};

class Enemy {
public:
    std::string enemyTexturePath;
    Rect sRect;
    FRect dRect;
    SDL_Texture * enemyTexture;
    SDL_FPoint direction;
    float speed = 0.0f;
    float acceleration = 0.0f;
    float maxSpeed = 0.0f;
    float detectionRange;
    int health;
    EnemyState state = Idle;

    Enemy(SDL_Renderer * renderer);

    virtual void setEnemyPosition(float x, float y);

    virtual void renderEnemy(SDL_Renderer * renderer);

    virtual void handleMovement(float deltaTime, const Room &room, Player &player);

    virtual void pathfinding(const Room &room, Player &player);

    virtual bool detectPlayer(const Player &player);

    virtual void attackPlayer(Player &player) = 0; // Pure virtual function

    virtual void takeDamage(int damage);

    virtual ~Enemy() = default;
};

#endif //SDL_BASEGAME_ENEMY_H
