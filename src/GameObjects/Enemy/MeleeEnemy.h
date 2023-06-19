#ifndef GORILLAGAME_MELEEENEMY_H
#define GORILLAGAME_MELEEENEMY_H

#include "BaseEnemy.h"

class MeleeEnemy : public BaseEnemy {
public:
    MeleeEnemy(float x, float y, float maxHp, std::vector<Pickup*>* pickup)
            : BaseEnemy(x, y, maxHp, pickup) {}

    // Override the attack method to represent melee attacking
    void attack() override;

    [[nodiscard]] bool inRadius() const override;
};

#endif //GORILLAGAME_MELEEENEMY_H
