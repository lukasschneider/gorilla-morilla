#include "Player.h"

Player::Player(SDL_Renderer *renderer, std::unique_ptr<Gun> gun) {
    this->gun = std::move(gun);
    dRect = {static_cast<float>(500), static_cast<float>(500), 64, 64};
    SDL_Surface *surface = IMG_Load(path.c_str());
    sRect = {0, 0, surface->w, surface->h};
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Player::renderPlayer(SDL_Renderer *renderer) {
    /*cout << "Player" << endl;
    cout << "x: " << dRect.x << endl;
    cout << "y: " << dRect.y << endl;*/
    SDL_RendererFlip flip = (dir == RIGHT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int screenWidth = 1280;
    int screenHeight = 720;
    SDL_FRect screenRect = {
            screenWidth / 2 - dRect.w / 2,
            screenHeight / 2 - dRect.h / 2,
            dRect.w,
            dRect.h
    };
    SDL_RenderCopyExF(renderer, texture, nullptr, &screenRect, 0.0, nullptr, flip);
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

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;

        // Increase speed up to maxSpeed
        speed += acceleration * deltaTime;
        if (speed > maxSpeed) {
            speed = maxSpeed;
        }

        float moveSpeed = speed * deltaTime;
        float newX = dRect.x + dirX * moveSpeed;
        float newY = dRect.y + dirY * moveSpeed;

        // Check horizontal movement
        if (!room.checkCollision({static_cast<int>(newX), static_cast<int>(dRect.y), static_cast<int>(dRect.w),
                                  static_cast<int>(dRect.h)})) {
            dRect.x = newX;
        } else {
            // If collision, move the player as close to the obstacle as possible
            while (!room.checkCollision(
                    {static_cast<int>(dRect.x + dirX), static_cast<int>(dRect.y), static_cast<int>(dRect.w),
                     static_cast<int>(dRect.h)})) {
                dRect.x += dirX;
            }
        }

        // Check vertical movement
        if (!room.checkCollision({static_cast<int>(dRect.x), static_cast<int>(newY), static_cast<int>(dRect.w),
                                  static_cast<int>(dRect.h)})) {
            dRect.y = newY;
        } else {
            // If collision, move the player as close to the obstacle as possible
            while (!room.checkCollision(
                    {static_cast<int>(dRect.x), static_cast<int>(dRect.y + dirY), static_cast<int>(dRect.w),
                     static_cast<int>(dRect.h)})) {
                dRect.y += dirY;
            }
        }

    } else {
        // If no movement input is given, slow down
        speed -= acceleration * deltaTime;
        if (speed < 0) {
            speed = 0;
        }
    }
}

void Player::setPlayerPosition(float x, float y) {
    this->dRect.x = x;
    this->dRect.y = y;
}


