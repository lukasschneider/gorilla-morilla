#include "Bullet.h"

void Bullet::update(float dt) {
    rect.x += cos(angle) * speed * dt;
    rect.y += sin(angle) * speed * dt;
}

void Bullet::render(SDL_Renderer *renderer, SDL_FRect *vp) const {
    SDL_FRect renderRect = {rect.x - vp->x, rect.y - vp->y, rect.w, rect.h};
    SDL_RenderCopyExF(renderer, texture, nullptr, &renderRect, angle * 180 / M_PI, nullptr, SDL_FLIP_NONE);
}

Bullet::Bullet([[maybe_unused]] float x, [[maybe_unused]] float y, float speed, float angle, SDL_Renderer *renderer,
               [[maybe_unused]] SDL_FRect *vp)
        : speed(speed), angle(angle), isActive(true){

    SDL_Surface *surface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    [[maybe_unused]] auto ratio = width/height;

    this->rect = {x + vp->x, y + vp->y, 50,static_cast<float>(25.0/ratio)};
}

void Bullet::deactivate() {
    isActive = false;
}

Bullet::~Bullet() {
    SDL_DestroyTexture(texture);
}
