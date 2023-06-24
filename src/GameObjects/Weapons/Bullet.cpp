#include "Bullet.h"

void Bullet::update(float dt) {
    rect.x += cos(angle) * speed * dt;
    rect.y += sin(angle) * speed * dt;
}

void Bullet::render(SDL_Renderer *renderer, SDL_FRect *vp) const {
    SDL_FRect renderRect = {rect.x - vp->x, rect.y - vp->y, rect.w, rect.h};
    SDL_RenderCopyExF(renderer, texture, NULL, &renderRect, angle * 180 / M_PI, NULL, SDL_FLIP_NONE);
}

Bullet::Bullet(float x, float y, float speed, float angle, SDL_Renderer *renderer, SDL_FRect *vp)
        : speed(speed), angle(angle), isActive(true){

    SDL_Surface *surface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    auto ratio = width/height;

    this->rect = {x + vp->x, y + vp->y, 50,static_cast<float>(25/ratio)};
}

void Bullet::deactivate() {
    isActive = false;
}

Bullet::~Bullet() {
    SDL_DestroyTexture(texture);
}
