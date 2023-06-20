#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H
#include "../lib/global.h"
#include "Weapons/Gun.h"
#include "../Map/Room.h"

class Room;

enum PlayerState{
    Dodge,
    Damage
};

enum Dir{
    LEFT,
    RIGHT
};

class Player {
public:
    std::string playerPath = BasePath "asset/graphic/player/a.png";
    std::string reloadPath = BasePath "asset/graphic/ui/tilemap_packed.png";
    Rect sRect;
    FRect dRect;
    Dir dir = LEFT;
    std::unique_ptr<Gun> gun;
    SDL_Texture * playerTexture;
    PlayerState state = Damage;
    float speed = 0.0f;
    float acceleration = 15000.0f;
    float maxSpeed = 650.0f;
    int health;
    int currency;
    SDL_FPoint rollDirection;
    float rollDuration = 0.7;
    float rollTimer = 0.0f;
    float rollMovementSpeed = 550.0f;
    bool isHit = false;
    float invincTimer = 1.0f;
    float timeSinceLastDamage = 0.0f;

    Player(SDL_Renderer * renderer, std::unique_ptr<Gun> gun);

    void setPlayerPosition(float x, float y);

    void takeDamage();

    void renderPlayer(SDL_Renderer * renderer);

    void handleMovement(const Uint8 *keyboardState, float deltaTime, const Room &room);

    int handleTeleport(const Room &room);
};


#endif //SDL_BASEGAME_PLAYER_H
