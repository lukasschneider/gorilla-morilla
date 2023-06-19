#include "BaseEnemy.h"
#include "../lib/ph.h"
#include "../lib/rh.h"

BaseEnemy::BaseEnemy(float x, float y, float maxHp, std::vector<Pickup *> *pickup)
        : dRect({x, y, 32, 32}), hp(maxHp), maxHp(maxHp), activePowerUps(pickup) {
    Renderer * renderer = RS::getInstance().get();
    dRect = {static_cast<float>(64),static_cast<float>(500),48,48};
    SDL_Surface * surface = IMG_Load(enemyPath.c_str());
    enemyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void BaseEnemy::update(float dt, Room &room) {


    if (hp <= 0) {
        respawn();
    }

    const float threshold = 0.0f;

    if (path && !path->empty()) {
        path->erase(path->begin());
        std::pair<int, int> destination = path->front();

        float destinationX = destination.first * TILE_SIZE + TILE_SIZE / 2.0f - dRect.w / 2.0f;
        float destinationY = destination.second * TILE_SIZE + TILE_SIZE / 2.0f - dRect.h / 2.0f;

        float dx = destinationX - dRect.x;
        float dy = destinationY - dRect.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= threshold) {
            path->erase(path->begin());

            if (!path->empty()) {
                destination = path->front();
                destinationX = destination.first * TILE_SIZE + TILE_SIZE / 2.0f - dRect.w / 2.0f;
                destinationY = destination.second * TILE_SIZE + TILE_SIZE / 2.0f - dRect.h / 2.0f;

                dx = destinationX - dRect.x;
                dy = destinationY - dRect.y;
                distance = std::sqrt(dx * dx + dy * dy);
            }
            else {
                return;
            }
        }

        dx /= distance;
        dy /= distance;

        float moveSpeed = speed * dt;
        float newX = dRect.x + dx * moveSpeed;
        float newY = dRect.y + dy * moveSpeed;

        if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(dRect.y),
                                  static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.x = newX;
        }

        if (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(newY),
                                  static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.y = newY;
        }
    }
}

void BaseEnemy::respawn() {
    die();
    dRect.x = 64;
    dRect.y = 64;
    hp = maxHp;
}

void BaseEnemy::render(SDL_Renderer *renderer, const SDL_FRect &viewport) {
    SDL_FRect enemyRect = {
            (dRect.x - viewport.x),
            (dRect.y - viewport.y),
            (dRect.w),
            (dRect.h)
    };
    SDL_RenderCopyF(renderer,enemyTexture, nullptr,&enemyRect);

    SDL_FRect hpBar = {
            dRect.x - viewport.x,
            dRect.y - viewport.y - 10,
            dRect.w * (hp / maxHp),
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

void BaseEnemy::coll(BulletRingBuffer &bullets) {
    for (int i = 0; i < bullets.size(); ++i) {
        Bullet *bullet = bullets.get(i);
        if (!bullet->isActive) continue;
        if (SDL_HasIntersectionF(&bullet->rect, &dRect)) {
            hp -= 20;
            bullet->deactivate();
        }
    }
}

bool BaseEnemy::PickupRate() {
    return rand() % 5 == 0;
}

void BaseEnemy::die() {
    if (PickupRate()) {
        SDL_Renderer *renderer = RS::getInstance().get();
        SDL_FRect tmp = {dRect.x + dRect.w / 2 - 16, dRect.y + dRect.h / 2 - 16, 32, 32};
        activePowerUps->push_back(new Banana(tmp, renderer));
    }
}



