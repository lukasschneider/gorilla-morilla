#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H
#include "../lib/global.h"
#include "Gun.h"

enum Dir{
    LEFT,
    RIGHT
};

class Player {
public:
    std::string path = BasePath "asset/graphic/player/a.png";
    Rect sRect;
    FRect dRect;
    Dir dir = LEFT;
    std::unique_ptr<Gun> gun;
    SDL_Texture * texture;
    float speed = 0.0f;
    float acceleration = 15000.0f;
    float maxSpeed = 750.0f;

    Player(SDL_Renderer * renderer, std::unique_ptr<Gun> gun);

    void renderPlayer(SDL_Renderer * renderer);

    void handleMovement(const Uint8 *keyboardState, float deltaTime, const Room &room);
    };


#endif //SDL_BASEGAME_PLAYER_H
