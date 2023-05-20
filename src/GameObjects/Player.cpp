#include "Player.h"



void Player::renderPlayer(SDL_Renderer *renderer) {
    if (dir == LEFT)
        SDL_RenderCopyEx(renderer, texture, &sRect, &dRect, 0.0, nullptr, SDL_FLIP_NONE);
    if (dir == RIGHT)
        SDL_RenderCopyEx(renderer, texture, &sRect, &dRect, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
}

void Player::handleMovement(const Uint8 *keyboardState, const float deltaTime) {
    float newX = dRect.x;
    float newY = dRect.y;

    float moveSpeed = static_cast<float>(speed) * (deltaTime / 1000.0f);
    //std::cout << "moveSpeed: " << moveSpeed << "\n";
    moveSpeed = 1;
    float dirX = 0.0f;
    float dirY = 0.0f;

    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_W]) {
        dirY -= 1.0f;  // adjust the direction
        cout << "UP";
    }
    if (keyboardState[SDL_SCANCODE_DOWN] || keyboardState[SDL_SCANCODE_S]) {
        dirY += 1.0f;// adjust the direction
        cout << "DOWN";
    }

    if (keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_A]) {
        dirX -= 1.0f;  // adjust the direction
        dir = Dir::LEFT;
    }
    if (keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_D]) {
        dirX += 1.0f;  // adjust the direction
        dir = Dir::RIGHT;
    }

    //

    float length = std::sqrt(dirX * dirX + dirY * dirY);
    cout << length << endl;
    if (length > 0.0f) {
        dirX /= length;
        dirY /= length;
        std::cout << "dirX: " << dirX << ", dirY: " << dirY << "\n";
        newX += dirX * moveSpeed;
        newY += dirY * moveSpeed;
        std::cout << "nX: " << newX << ", nY: " << newY << "\n";
    }

    //std::cout << "newX: " << newX << ", newY: " << newY << "\n";

    // Update the player's position directly
    dRect.x = static_cast<int>(newX);
    dRect.y = static_cast<int>(newY);
}
