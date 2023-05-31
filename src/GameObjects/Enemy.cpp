#include "Enemy.h"

Enemy::Enemy(float x, float y, float maxHp)
        : body({x, y, 50, 50}), hp(maxHp), maxHp(maxHp) { }

void Enemy::update() {
    if (hp <= 0) {
        respawn();
    }
}

void Enemy::respawn() {
    cout<< "respawn" <<endl;
    // Change these values to set the new enemy's position
    body.x = 500;
    body.y = 500;

    hp = maxHp;
}

void Enemy::render(SDL_Renderer* renderer, const SDL_FRect &viewport) {

    SDL_Rect enemyRect = {
            static_cast<int>(body.x - viewport.x),
            static_cast<int>(body.y - viewport.y),
            static_cast<int>(body.w),
            static_cast<int>(body.h)
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &enemyRect);

    // Draw HP bar
    SDL_FRect hpBar = {
            body.x - viewport.x,
            body.y - viewport.y - 10,
            body.w * (hp / maxHp),
            5
    };
    SDL_Rect hpBarRect = {
            static_cast<int>(hpBar.x),
            static_cast<int>(hpBar.y),
            static_cast<int>(hpBar.w),
            static_cast<int>(hpBar.h)
    };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &hpBarRect);
}

void Enemy::coll(const std::vector<Bullet>& bullets) {
    for(const auto& bullet : bullets){
        if(SDL_HasIntersectionF(&bullet.rect, &body)){
            hp -= 25;
        }
    }
}


