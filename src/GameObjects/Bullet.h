#ifndef GORILLAGAME_BULLET_H
#define GORILLAGAME_BULLET_H

#include "../lib/global.h"

class Bullet {
public:
    std::string path = BasePath "asset/graphic/bullets/basic_bullet.png";

    SDL_FRect rect;
    SDL_Texture* texture;
    float speed;
    float angle;

    Bullet(float x, float y, float speed, float angle, SDL_Renderer* renderer);

    void update(float dt);

    void render(SDL_Renderer* renderer) const;
};

#endif //GORILLAGAME_BULLET_H