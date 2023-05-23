#ifndef SDL_BASEGAME_CAMERA_H
#define SDL_BASEGAME_CAMERA_H

#include "../lib/global.h"

struct Camera {
    SDL_FRect frect; // float rectangle for camera
    int w;
    int h;

    void update(const SDL_FRect &playerRect, int windowWidth, int windowHeight);
};

// Camera update function
void Camera::update(const SDL_FRect& playerRect, int windowWidth, int windowHeight) {
    // Center the camera over the player
    frect.x = playerRect.x + playerRect.w / 2 - w / 2;
    frect.y = playerRect.y + playerRect.h / 2 - h / 2;

    // Clamp the camera to the room boundaries
    frect.x = std::max(static_cast<float>(0), std::min(frect.x, static_cast<float>(windowWidth - w)));
    frect.y = std::max(static_cast<float>(0), std::min(frect.y, static_cast<float>(windowHeight - h)));
}

#endif //SDL_BASEGAME_CAMERA_H
