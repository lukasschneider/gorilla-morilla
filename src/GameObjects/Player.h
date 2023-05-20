#ifndef SDL_BASEGAME_PLAYER_H
#define SDL_BASEGAME_PLAYER_H

#include "../lib/global.h"

enum Dir{
    LEFT,
    RIGHT
};

class Player {
public:
    std::string path = BasePath "asset/graphic/player/ape.png";
    Rect sRect;
    Rect dRect;
    Dir dir = LEFT;
    SDL_Texture * texture;
    float speed = 500;

    Player(SDL_Renderer * renderer){
        dRect = {700,700,128,128};
        SDL_Surface * surface = IMG_Load(path.c_str());
        sRect = {0, 0, surface->w, surface->h};
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    void renderPlayer(SDL_Renderer * renderer);
    void handleMovement(const Uint8 *keyboardState, const float deltaTime);
    };


#endif //SDL_BASEGAME_PLAYER_H
