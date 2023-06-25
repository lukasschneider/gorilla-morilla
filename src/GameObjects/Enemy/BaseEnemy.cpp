#include "BaseEnemy.h"


BaseEnemy::BaseEnemy(float x, float y, float maxHp, std::vector<Pickup *> *pickup)
        : dRect({x, y, 32, 32}), hp(maxHp), maxHp(maxHp), activePowerUps(pickup) {
    Renderer * renderer = RS::getInstance().get();
    dRect = {static_cast<float>(64),static_cast<float>(500),48,48};
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

void BaseEnemy::update(float dt, Room &room) {


    if (hp <= 0) {
        respawn();
    }

    if (isHit) {
        hitTime += dt;
        if (hitTime >= hitDuration) {
            hitTime = 0.0f;
            isHit = false;
        }
    }


    const float threshold = 0.0f;

    if (path && !path->empty()) {
        path->erase(path->begin());
        std::pair<int, int> destination = path->front();

        float destinationX = static_cast<float>(destination.first) * TILE_SIZE + TILE_SIZE / 2.0f - dRect.w / 2.0f;
        float destinationY = static_cast<float>(destination.second) * TILE_SIZE + TILE_SIZE / 2.0f - dRect.h / 2.0f;

        float dx = destinationX - dRect.x;
        float dy = destinationY - dRect.y;

        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance <= threshold) {
            path->erase(path->begin());

            if (!path->empty()) {
                destination = path->front();
                destinationX = static_cast<float>(destination.first) * TILE_SIZE + TILE_SIZE / 2.0f - dRect.w / 2.0f;
                destinationY = static_cast<float>(destination.second) * TILE_SIZE + TILE_SIZE / 2.0f - dRect.h / 2.0f;

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

void BaseEnemy::render(const SDL_FRect &viewport) {
    Renderer * renderer = RS::getInstance().get();
    if (isHit) {
        int alpha = static_cast<int>(255 * (0.5f + 0.5f * sin(10.0f * hitTime))); // Flashing effect
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

[[maybe_unused]] void BaseEnemy::coll(BulletRingBuffer &bullets) {
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

bool BaseEnemy::PickupRate() {
    return rand() % 10 == 0;
}

void BaseEnemy::die() {
    if (PickupRate()) {
        SDL_Renderer *renderer = RS::getInstance().get();
        SDL_FRect tmp = {dRect.x + dRect.w / 2 - 16, dRect.y + dRect.h / 2 - 16, 32, 32};
        activePowerUps->push_back(new Banana(tmp, renderer));
    }
}

void BaseEnemy::getHit() {
    isHit = true;
}

/*bool BaseEnemy::lineOfSightCheck(const Player& player, const std::vector<std::vector<int>>& collisionLayer) {
    // Calculate the start and end points in tile coordinates
    int startX = static_cast<int>(dRect.x / TILE_SIZE);
    int startY = static_cast<int>(dRect.y / TILE_SIZE);
    int endX = static_cast<int>(player.dRect.x / TILE_SIZE);
    int endY = static_cast<int>(player.dRect.y / TILE_SIZE);

    // Bresenham's Line Algorithm
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);

    int sx = (startX < endX) ? 1 : -1;
    int sy = (startY < endY) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        if (startX == endX && startY == endY) {
            return true;
        }

        if (collisionLayer[startY][startX] != -1) {
            return false;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            startX += sx;
        }
        if (e2 < dx) {
            err += dx;
            startY += sy;
        }
    }
}

*/



