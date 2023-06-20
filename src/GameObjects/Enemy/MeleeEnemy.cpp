//
// Created by Jonas Luven on 19.06.23.
//

#include "MeleeEnemy.h"
#include "../../lib/ph.h"

void MeleeEnemy::attack() {
    Player* p = PS::getInstance().get();
    p->takeDamage();
}


bool MeleeEnemy::inRadius() const {
    Player* p = PS::getInstance().get();
    float dx = (dRect.x + dRect.w / 2) - (p->dRect.x + p->dRect.w / 2);
    float dy = (dRect.y + dRect.h / 2) - (p->dRect.y + p->dRect.h / 2);
    float distance = std::sqrt(dx*dx + dy*dy);
    return distance <= radius;
}


