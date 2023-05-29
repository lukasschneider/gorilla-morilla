#ifndef GORILLAGAME_GUN_H
#define GORILLAGAME_GUN_H

#include "../lib/global.h"

class Gun {
public:
    std::string path = BasePath "asset/graphic/guns/ak.png";
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_FRect dstRect;
    SDL_FPoint gunMountOffset = {0.5f, 0.5f}; // Relative position to the player (range 0 to 1)
    SDL_FPoint bulletExitOffset = {1.0f, 0.5f}; // Relative position on the gun (range 0 to 1)

    float angle = 0.0f;

    Gun(SDL_Renderer * render);

    SDL_FPoint getBulletExitPosition() const;

    void render(SDL_Renderer *renderer, const SDL_FRect &playerRect, const SDL_FRect &viewport);

    void updateAngle(int mouseX, int mouseY, const SDL_FRect &playerRect, const SDL_FRect &viewport);
};



#endif //GORILLAGAME_GUN_H
