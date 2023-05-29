#include "Gun.h"

Gun::Gun(SDL_Renderer *render) {
    int width, height;

    SDL_Surface * surface = IMG_Load(path.c_str());
    srcRect = {0, 0, surface->w, surface->h};
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    auto ratio = width/height;
    dstRect = { 0, 0, 64,static_cast<float>(64/ratio)};
}

SDL_FPoint Gun::getBulletExitPosition() const {
    float exitX = dstRect.x + dstRect.w * bulletExitOffset.x;
    float exitY = dstRect.y + dstRect.h * bulletExitOffset.y;
    return {exitX, exitY};
}

void Gun::render(SDL_Renderer *renderer, const SDL_FRect &playerRect, const SDL_FRect &viewport) {
    float centerX = playerRect.x + playerRect.w * gunMountOffset.x;
    float centerY = playerRect.y + playerRect.h * gunMountOffset.y;

    dstRect.x = centerX - dstRect.w * bulletExitOffset.x;
    dstRect.y = centerY - dstRect.h * bulletExitOffset.y;

    SDL_Rect dstRectInt = {static_cast<int>(dstRect.x), static_cast<int>(dstRect.y), static_cast<int>(dstRect.w), static_cast<int>(dstRect.h)};
    SDL_Point center = {static_cast<int>(dstRect.w * gunMountOffset.x), static_cast<int>(dstRect.h * gunMountOffset.y)};
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRectInt, angle, &center, SDL_FLIP_NONE);

    // Calculate line start point
    SDL_Point lineStart;
    lineStart.x = centerX - viewport.x;
    lineStart.y = centerY - viewport.y;

    // Calculate line end point based on angle
    SDL_Point lineEnd;
    lineEnd.x = lineStart.x + 1000 * cos(angle * M_PI / 180);
    lineEnd.y = lineStart.y + 1000 * sin(angle * M_PI / 180);

    // Render line
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, lineStart.x, lineStart.y, lineEnd.x, lineEnd.y);
}




void Gun::updateAngle(int mouseX, int mouseY, const SDL_FRect &playerRect, const SDL_FRect &camera) {
    float centerX = playerRect.x + playerRect.w * gunMountOffset.x - camera.x;
    float centerY = playerRect.y + playerRect.h * gunMountOffset.y - camera.y;

    float deltaX = mouseX - centerX - camera.x;
    float deltaY = mouseY - centerY -camera.y;

    angle = atan2(-deltaY, deltaX) * 180 / M_PI;

}


