#include "MeleeEnemy.h"

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


void MeleeEnemy::render(const SDL_FRect &viewport) {
    if (knife) { // make sure the knife exists
        knife->renderKnife(viewport);
    }
    BaseEnemy::render(viewport);

    // Draw the circle (radius)
    //Renderer * render = RS::getInstance().get();


}


