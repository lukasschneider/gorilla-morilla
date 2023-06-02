#ifndef GORILLAGAME_GUN_H
#define GORILLAGAME_GUN_H

#include "../lib/global.h"
#include "Bullet.h"

class Gun {
public:
    std::string path = BasePath "asset/graphic/guns/ak.png";
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_FRect dstRect;
    SDL_FPoint gunMountOffset = {1.0f, 1.0f};
    SDL_FPoint bulletExitOffset = {1.0f, 0.5f};

    float timeSinceLastShot = 0.0f;
    const float shotDelay = 0.28f;

    float angle = 0.0f;

    std::vector<Bullet> bullets = {};

    Gun(SDL_Renderer * render);

    SDL_FPoint getBulletExitPosition() const;

    void render(SDL_Renderer *renderer);

    void updateAngle(int mouseX, int mouseY, const SDL_FRect &playerRect, const SDL_FRect &viewport);

    // Bullet Logic
    void fire(Renderer* renderer, SDL_FRect *vp);

    void updateBullets(float dt);

    void renderBullets(SDL_Renderer *renderer, SDL_FRect *vp);
};



#endif //GORILLAGAME_GUN_H
