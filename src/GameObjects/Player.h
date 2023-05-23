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
    FRect dRect;
    Dir dir = LEFT;
    SDL_Texture * texture;
    float speed = 600;

    Player(SDL_Renderer * renderer){
        int windowWidth, windowHeight;
        SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);
        windowWidth /= 2;
        windowHeight /= 2;
        dRect = {static_cast<float>(windowWidth),static_cast<float>(windowHeight),100,100};
        SDL_Surface * surface = IMG_Load(path.c_str());
        sRect = {0, 0, surface->w, surface->h};
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
    void renderPlayer(SDL_Renderer * renderer);
    void handleMovement(const Uint8 *keyboardState, float deltaTime, const Room &room);
    };


#endif //SDL_BASEGAME_PLAYER_H
