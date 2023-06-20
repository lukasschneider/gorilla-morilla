//
// Created by Jonas Luven on 19.06.23.
//

#include "MeleeEnemy.h"
#include "../../lib/ph.h"

MeleeEnemy::MeleeEnemy(float x, float y, float maxHp, std::vector<Pickup *> *pickup)
        : BaseEnemy(x, y, maxHp, pickup),
          knife(std::make_unique<Knife>())
{
}

void MeleeEnemy::attack() {
    Player* p = PS::getInstance().get();
    p->takeDamage();
    //knife->slash();
}


bool MeleeEnemy::inRadius() const {
    Player* p = PS::getInstance().get();
    float dx = (dRect.x + dRect.w / 2) - (p->dRect.x + p->dRect.w / 2);
    float dy = (dRect.y + dRect.h / 2) - (p->dRect.y + p->dRect.h / 2);
    float distance = std::sqrt(dx*dx + dy*dy);
    return distance <= radius;
}

void MeleeEnemy::attackUpdate() {
    if(inRadius())
        attack();
}

void MeleeEnemy::update(float dt, Room &room) {
    BaseEnemy::update(dt,room);

    if (knife) {
        knife->updateKnife(dRect, dt);
    }
    attackUpdate();
}
#include <cmath>

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void MeleeEnemy::render(const SDL_FRect &viewport) {
    if (knife) { // make sure the knife exists
        knife->renderKnife(viewport);
    }
    BaseEnemy::render(viewport);

    // Draw the circle (radius)
    Renderer * render = RS::getInstance().get();
    int centerX = static_cast<int>(dRect.x + dRect.w / 2 - viewport.x);
    int centerY = static_cast<int>(dRect.y + dRect.h / 2 - viewport.y);
    int radius = static_cast<int>(this->radius);

    // Set the color of the radius (here it is set to red, you can change it as needed)
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255); // RGBA
    // Draw the circle
    drawCircle(render, centerX, centerY, radius);
    // Reset the render color
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Black

}


