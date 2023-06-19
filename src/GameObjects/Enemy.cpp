#include "Enemy.h"
#include "../lib/ph.h"

void SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    for (double dy = 1; dy <= radius; dy += 1.0) {
        double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
        SDL_RenderDrawLine(renderer, x-dx, y+dy-radius, x+dx, y+dy-radius);
        SDL_RenderDrawLine(renderer, x-dx, y-dy+radius, x+dx, y-dy+radius);
    }
}

Enemy::Enemy(float x, float y, float maxHp, std::vector<Pickup *> *pickup)
        : body({x, y, 32, 32}), hp(maxHp), maxHp(maxHp), activePowerUps(pickup) {}

void Enemy::update(float dt, Room &room) {

    if (hp <= 0) {
        respawn();
    }

    const float threshold = 0.0f;

    if (path && !path->empty()) {
        path->erase(path->begin());
        std::pair<int, int> destination = path->front();

        float destinationX = destination.first * TILE_SIZE + TILE_SIZE / 2.0f - body.w / 2.0f;
        float destinationY = destination.second * TILE_SIZE + TILE_SIZE / 2.0f - body.h / 2.0f;

        // Compute the direction vector
        float dx = destinationX - body.x;
        float dy = destinationY - body.y;

        // Compute the distance to the destination
        float distance = std::sqrt(dx * dx + dy * dy);

        // If the enemy is close enough to the destination, move on to the next point
        if (distance <= threshold) {
            path->erase(path->begin());

            if (!path->empty()) {
                destination = path->front();
                destinationX = destination.first * TILE_SIZE + TILE_SIZE / 2.0f - body.w / 2.0f;
                destinationY = destination.second * TILE_SIZE + TILE_SIZE / 2.0f - body.h / 2.0f;

                dx = destinationX - body.x;
                dy = destinationY - body.y;
                distance = std::sqrt(dx * dx + dy * dy);
            }
            else {
                // The path is empty, the enemy can stand still
                return;
            }
        }

        // Normalize the direction vector
        dx /= distance;
        dy /= distance;

        float moveSpeed = speed * dt;
        float newX = body.x + dx * moveSpeed;
        float newY = body.y + dy * moveSpeed;

        // Perform collision checks and update the position if there is no collision
        if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(body.y),
                                  static_cast<int>(body.w), static_cast<int>(body.h)})) {
            body.x = newX;
        }

        if (!room.checkCollision({static_cast<int>(body.x), static_cast<int>(newY),
                                  static_cast<int>(body.w), static_cast<int>(body.h)})) {
            body.y = newY;
        }
    }


    if (inRadius()){
        attack();
    }
}

void Enemy::respawn() {
    die();
    body.x = 64;
    body.y = 64;

    hp = maxHp;
}

void Enemy::render(SDL_Renderer *renderer, const SDL_FRect &viewport) {

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

void Enemy::coll(BulletRingBuffer &bullets) {
    for (int i = 0; i < bullets.size(); ++i) {
        Bullet *bullet = bullets.get(i);
        if (!bullet->isActive) continue; // Skip inactive bullets
        if (SDL_HasIntersectionF(&bullet->rect, &body)) {
            hp -= 20;
            bullet->deactivate(); // Deactivate the bullet on hit
        }
    }
}

bool Enemy::spawnrate() {
    return rand() % 1 == 0;
}

void Enemy::die() {
    if (spawnrate()) {
        SDL_Renderer *renderer = RS::getInstance().get();
        SDL_FRect tmp = {body.x + body.w / 2 - 16, body.y + body.h / 2 - 16, 32, 32};
        activePowerUps->push_back(new Banana(tmp, renderer));
    }
}


Enemy::~Enemy() {
    for (auto powerUp : *activePowerUps) {
        delete powerUp;
    }
    activePowerUps->clear();
}

void Enemy::attack() {
    Player* p = PS::getInstance().get();
    p->takeDamage();
}


bool Enemy::inRadius() const {
    Player* p = PS::getInstance().get();
    float dx = (body.x + body.w / 2) - (p->dRect.x + p->dRect.w / 2);
    float dy = (body.y + body.h / 2) - (p->dRect.y + p->dRect.h / 2);
    float distance = std::sqrt(dx*dx + dy*dy);
    return distance <= radius;
}

