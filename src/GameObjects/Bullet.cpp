//
// Created by user on 30.05.23.
//

#include "Bullet.h"

void Bullet::update(float dt) {
    rect.x += cos(angle) * speed * dt;
    rect.y += sin(angle) * speed * dt;
}

void Bullet::render(SDL_Renderer *renderer) const {
    SDL_Rect bulletRect = {static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.w), static_cast<int>(rect.h)};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &bulletRect);
}

Bullet::Bullet(float x, float y, float speed, float angle)
        : rect({x, y, 5, 5}), speed(speed), angle(angle) { }
