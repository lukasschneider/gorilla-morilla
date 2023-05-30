#ifndef GORILLAGAME_BULLET_H
#define GORILLAGAME_BULLET_H

#include "../lib/global.h"

class Bullet {
public:
    SDL_FRect rect;
    float speed;
    float angle;

    Bullet(float x, float y, float speed, float angle);

    void update(float dt);

    void render(SDL_Renderer* renderer) const;
};



#endif //GORILLAGAME_BULLET_H
