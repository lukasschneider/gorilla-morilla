#ifndef GORILLAGAME_BULLET_H
#define GORILLAGAME_BULLET_H

#include "../../lib/global.h"
class Bullet {
public:
    std::string path = BasePath "asset/graphic/bullets/basic_bullet.png";
    SDL_FRect rect{};
    SDL_Texture* texture;
    float speed;
    float angle;
    bool isActive;

    Bullet([[maybe_unused]] float x, [[maybe_unused]] float y, float speed, float angle, SDL_Renderer* renderer,
           [[maybe_unused]] SDL_FRect *vp);

    void deactivate();

    void update(float dt);

    void render(SDL_Renderer *renderer, SDL_FRect *vp) const;

    ~Bullet();
};

#endif //GORILLAGAME_BULLET_H