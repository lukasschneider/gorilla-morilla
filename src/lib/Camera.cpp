#include "Camera.h"

Camera::Camera(Player *player) {
    // Lukas is kein Ultra :(
    this->player = player;
    this->viewport = {
            this->viewport.x = static_cast<int>(player->dRect.x + player->dRect.w / 2 - 1280 / 2),
            this->viewport.y = static_cast<int>(player->dRect.y + player->dRect.h / 2 - 720 / 2),
            this->viewport.w = 1280,
            this->viewport.h = 720
    };
}

void Camera::updateCamera() {
    this->viewport.x = static_cast<int>(player->dRect.x + player->dRect.w / 2 - 1280 / 2);
    this->viewport.y = static_cast<int>(player->dRect.y + player->dRect.h / 2 - 720 / 2);
}

void Camera::renderCam(Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // set the draw color to red

    SDL_RenderDrawRect(renderer, &this->viewport); // draw the outline

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // reset the draw color to black
}


