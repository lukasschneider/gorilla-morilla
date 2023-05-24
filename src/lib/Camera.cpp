#include "Camera.h"

Camera::Camera(Player *player) {
    // Lukas is kein Ultra :(
    this->player = player;
    this->viewport = {
            .x = static_cast<int>(player->dRect.x + player->dRect.w / 2 - 1920 / 2),
            .y = static_cast<int>(player->dRect.y + player->dRect.h / 2 - 1080 / 2),
            .w = 1920,
            .h = 1080
    };
}

void Camera::updateCamera() {
    this->viewport.x = -static_cast<int>(player->dRect.x + player->dRect.w / 2 - 1920 / 2);
    this->viewport.y = -static_cast<int>(player->dRect.y + player->dRect.h / 2 - 1080 / 2);
}

void Camera::renderCam(Renderer *render) {
    SDL_RenderSetViewport(render,&this->viewport);
}


