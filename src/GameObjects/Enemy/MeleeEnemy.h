#ifndef GORILLAGAME_MELEEENEMY_H
#define GORILLAGAME_MELEEENEMY_H

#include "BaseEnemy.h"
#include "../../GameObjects/Weapons/Knife.h"

class MeleeEnemy : public BaseEnemy {
public:
    float radius = 75.0f;
    std::unique_ptr<Knife> knife;
    MeleeEnemy(float x, float y, float maxHp, std::vector<Pickup*>* pickup);

    void update(float dt, Room &room) override;

    void attackUpdate();

    void render(const SDL_FRect &viewport) override;

    void attack() override;

    [[nodiscard]] bool inRadius() const override;
};

#endif //GORILLAGAME_MELEEENEMY_H
