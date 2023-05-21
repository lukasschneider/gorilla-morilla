#include "Player.h"


void Player::renderPlayer(SDL_Renderer *renderer) {
    SDL_RendererFlip flip = (dir == RIGHT) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyExF(renderer, texture, nullptr, &dRect, 0.0, nullptr, flip);
}

void Player::handleMovement(const Uint8* keyboardState, float deltaTime) {
    float newX = dRect.x;
    float newY = dRect.y;

    float moveSpeed = speed * deltaTime;
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
        newX += dirX * moveSpeed;
        newY += dirY * moveSpeed;
    }

    dRect.x = newX;
    dRect.y = newY;
}
