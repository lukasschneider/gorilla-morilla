#include "Enemy.h"
#include "../lib/ph.h"


Enemy::Enemy(float x, float y, float maxHp, std::vector<Pickup *> *pickup)
        : dRect({x, y, 48, 48}), knife(std::make_unique<Knife>()), hp(maxHp), maxHp(maxHp), activePowerUps(pickup) {
    Renderer * renderer = RS::getInstance().get();
    SDL_Surface * surface = IMG_Load(enemyPath.c_str());
    if (!surface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    enemyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!enemyTexture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(surface);
}
void Enemy::update(float dt, Room &room) {
    if (knife) {
        knife->updateKnife(dRect, dt);
    }
    attackUpdate();
    if (hp <= 0) {
        Enemy *e = die();
        auto it = std::find(room.enemies.begin(), room.enemies.end(), e);
        if (it != room.enemies.end()) {
            room.enemies.erase(it);
        }
    }

    if (isHit) {
        hitTime += dt;
        if (hitTime >= hitDuration) {
            hitTime = 0.0f;
            isHit = false;
        }
    }


    const float threshold = 32.0f;

    if (path && !path->empty()) {
        path->erase(path->begin());
        std::pair<int, int> destination = path->front();

        float destinationX = destination.first * TILE_SIZE + TILE_SIZE / 2.0f - dRect.w / 2.0f;
        float destinationY = destination.second * TILE_SIZE + TILE_SIZE / 2.0f - dRect.h / 2.0f;

        // Compute the direction vector
        float dx = destinationX - dRect.x;
        float dy = destinationY - dRect.y;

        // Compute the distance to the destination
        float distance = std::sqrt(dx * dx + dy * dy);

        // If the enemy is close enough to the destination, move on to the next point
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

        // Normalize the direction vector
        dx /= distance;
        dy /= distance;

        float moveSpeed = speed * dt;
        float newX = dRect.x + dx * moveSpeed;
        float newY = dRect.y + dy * moveSpeed;

        // Perform collision checks and update the position if there is no collision
        if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(dRect.y),
                                  static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.x = newX;
        }

        if (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(newY),
                                  static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.y = newY;
        }
    }


    if (inRadius()){
        attack();
    }
}

void Enemy::respawn() {
    die();
    dRect.x = 64;
    dRect.y = 64;

    hp = maxHp;
}

void Enemy::render(SDL_Renderer *renderer, const SDL_FRect &viewport) {
    if (knife) { // make sure the knife exists
        knife->renderKnife(viewport);
    }
    //Renderer * renderer = RS::getInstance().get();
    if (isHit) {
        int alpha = 255 * (0.5f + 0.5f * sin(10.0f * hitTime)); // Flashing effect
        SDL_SetTextureAlphaMod(enemyTexture, alpha);
    } else {
        SDL_SetTextureAlphaMod(enemyTexture, 255);
    }
    SDL_FRect enemyRect = {
            (dRect.x - viewport.x),
            (dRect.y - viewport.y),
            (dRect.w),
            (dRect.h)
    };
    SDL_RenderCopyF(renderer,enemyTexture, nullptr,&enemyRect);

    // Render total health bar in red
    SDL_FRect totalHealthBar = {
            dRect.x - viewport.x,
            dRect.y - viewport.y - 10,
            dRect.w,
            5
    };
    SDL_Rect totalHealthBarRect = {
            static_cast<int>(totalHealthBar.x),
            static_cast<int>(totalHealthBar.y),
            static_cast<int>(totalHealthBar.w),
            static_cast<int>(totalHealthBar.h)
    };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
    SDL_RenderFillRect(renderer, &totalHealthBarRect);

// Render current health bar in green
    SDL_FRect currentHealthBar = {
            dRect.x - viewport.x,
            dRect.y - viewport.y - 10,
            dRect.w * (hp / maxHp),
            5
    };
    SDL_Rect currentHealthBarRect = {
            static_cast<int>(currentHealthBar.x),
            static_cast<int>(currentHealthBar.y),
            static_cast<int>(currentHealthBar.w),
            static_cast<int>(currentHealthBar.h)
    };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
    SDL_RenderFillRect(renderer, &currentHealthBarRect);
}

void Enemy::coll(BulletRingBuffer &bullets) {
    for (int i = 0; i < bullets.size(); ++i) {
        Bullet *bullet = bullets.get(i);
        if (!bullet->isActive) continue;
        if (SDL_HasIntersectionF(&bullet->rect, &dRect) /*&& !isHit*/) {
            hp -= 20;
            bullet->deactivate();
            getHit();
        }
    }
}

void Enemy::getHit() {
    isHit = true;
}

bool Enemy::spawnrate() {
    return rand() % 2 == 0;
}

Enemy* Enemy::die() {
    if (spawnrate()) {
        SDL_Renderer *renderer = RS::getInstance().get();
        SDL_FRect tmp = {dRect.x + dRect.w / 2 - 16, dRect.y + dRect.h / 2 - 16, 32, 32};
        activePowerUps->push_back(new Banana(tmp, renderer));
    }
    return this;
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
    float dx = (dRect.x + dRect.w / 2) - (p->dRect.x + p->dRect.w / 2);
    float dy = (dRect.y + dRect.h / 2) - (p->dRect.y + p->dRect.h / 2);
    float distance = std::sqrt(dx*dx + dy*dy);
    return distance <= radius;
}


void Enemy::attackUpdate() const {
    if(inRadius())
        attack();
}

