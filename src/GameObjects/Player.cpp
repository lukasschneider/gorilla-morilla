#include "Player.h"

Player::Player(SDL_Renderer *renderer, std::unique_ptr<Gun> gun) : gun(std::move(gun)), health(6), currency(5){

    dRect = {static_cast<float>(448),static_cast<float>(700),60,60};
    SDL_Surface * surface = IMG_Load(playerPath.c_str());
    sRect = {0, 0, surface->w, surface->h};
    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Player::renderPlayer(SDL_Renderer *renderer) {
    SDL_RendererFlip flip = (dir == RIGHT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int screenWidth = 1280;
    int screenHeight = 720;
    SDL_FRect screenRect = {
            screenWidth / 2 - dRect.w / 2,
            screenHeight / 2 - dRect.h / 2,
            dRect.w,
            dRect.h
    };
    double angle = 0.0;
    if (state == PlayerState::Dodge) {
        angle = (rollDuration - rollTimer) / rollDuration * 360.0;
    }
    SDL_RenderCopyExF(renderer, playerTexture, nullptr, &screenRect, angle, nullptr, flip);
}

int Player::handleTeleport(const Room &room) {
    const SDL_Rect rect{
            static_cast<int>(dRect.x),
            static_cast<int>(dRect.y),
            static_cast<int>(dRect.w),
            static_cast<int>(dRect.h)
    };
    switch (room.checkTeleport(rect)) {
        case TELEPORT_TOP:
            return TELEPORT_TOP;
        case TELEPORT_RIGHT:
            return TELEPORT_RIGHT;
        case TELEPORT_BOTTOM:
            return TELEPORT_BOTTOM;
        case TELEPORT_LEFT:
            return TELEPORT_LEFT;
    }
    return 0;
}

void Player::handleMovement(const Uint8 *keyboardState, float deltaTime, const Room &room) {
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        dirY -= 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        dirY += 1.0f;
    }
    if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        dirX -= 1.0f;
        dir = LEFT;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        dirX += 1.0f;
        dir = RIGHT;
    }
    if (keyboardState[SDL_SCANCODE_LSHIFT] && state != PlayerState::Dodge) {
        state = PlayerState::Dodge;
        rollTimer = rollDuration;
        if (dirX == 0.0f && dirY == 0.0f) {
            rollDirection = (dir == RIGHT) ? SDL_FPoint{1.0f, 0.0f} : SDL_FPoint{-1.0f, 0.0f};
        } else {
            rollDirection = {dirX, dirY};
        }
    }

    if (state == PlayerState::Dodge) {
        rollTimer -= deltaTime;
        if (rollTimer <= 0.0f) {
            state = PlayerState::Damage;
        } else {
            float rollSpeed = rollMovementSpeed * deltaTime;
            float newX = dRect.x + rollDirection.x * rollSpeed;
            float newY = dRect.y + rollDirection.y * rollSpeed;

            if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(dRect.y),
                                      static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
                dRect.x = newX;
            }

            if (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(newY),
                                      static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
                dRect.y = newY;
            }


            return;
        }

    }

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;

        speed += acceleration * deltaTime;

        if (speed > maxSpeed) {
            speed = maxSpeed;
        }

        float moveSpeed = speed * deltaTime;
        float newX = dRect.x + dirX * moveSpeed;
        float newY = dRect.y + dirY * moveSpeed;

        if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(dRect.y),
                                  static_cast<int>(dRect.w), static_cast<int>(dRect.h)})) {
            dRect.x = newX;
        } else {
            while (!room.checkCollision(
                    {static_cast<int>(dRect.x + dirX), static_cast<int>(dRect.y), static_cast<int>(dRect.w),
                     static_cast<int>(dRect.h)})) {
                dRect.x += dirX;
            }
        }

        if (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(newY), static_cast<int>(dRect.w),
                                  static_cast<int>(dRect.h)})) {
            dRect.y = newY;
        } else {
            while (!room.checkCollision(
                    {static_cast<int>(dRect.x), static_cast<int>(dRect.y + dirY), static_cast<int>(dRect.w),
                     static_cast<int>(dRect.h)})) {
                dRect.y += dirY;
            }
        }

    } else {
        speed -= acceleration * deltaTime;
        if (speed < 0) {
            speed = 0;
        }
    }
    if (isHit) {
        timeSinceLastDamage += deltaTime;

        if (timeSinceLastDamage >= invincTimer) {
            isHit = false;
        }
    }
}

void Player::takeDamage() {
    if (state != PlayerState::Dodge && !isHit) {
        health --;
        isHit = true;
        timeSinceLastDamage = 0;
    }
}
