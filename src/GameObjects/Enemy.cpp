#include "Enemy.h"

Enemy::Enemy(float x, float y, float maxHp)
        : body({x, y, 50, 50}), hp(maxHp), maxHp(maxHp) { }

void Enemy::update(float dt) {

    if (hp <= 0) {
        respawn();
    }

    if (body.x + body.w >= 14*64) {
        movingRight = false;
    }
        // If the enemy hits the left boundary, change direction
    else if (body.x <= 128) {
        movingRight = true;
    }

    // Update the position based on the direction
    if (movingRight) {
        body.x += speed * dt;
    } else {
        body.x -= speed * dt;
    }
}

void Enemy::respawn() {
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

void Enemy::coll(std::vector<Bullet*>& bullets) {
    for(auto& bullet : bullets){
        if(SDL_HasIntersectionF(&bullet->rect, &body)){
            // Check if the bullet has already hit this enemy
            if (std::find(hitBullets.begin(), hitBullets.end(), bullet) == hitBullets.end()) {
                hp -= 20;
                hitBullets.push_back(bullet);
            }
        }
    }
}




