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

void Gun::updateAngle(int mouseX, int mouseY, const SDL_FRect &playerRect, const SDL_FRect &viewport) {
    float centerX = playerRect.x + playerRect.w / 2 - viewport.x;
    float centerY = playerRect.y + playerRect.h / 2 - viewport.y;

    // Adjust the gun's position to the center of the player.
    dstRect.x = centerX - dstRect.w / 2;
    dstRect.y = centerY - dstRect.h / 2;

    float deltaX = mouseX - centerX;
    float deltaY = mouseY - centerY;

    // Assuming that the right direction (positive x-axis) is the reference.
    float refX = 1.0f, refY = 0.0f;

    // Dot product of reference vector and the vector to the mouse.
    float dot = refX * deltaX + refY * deltaY;
    // Cross product of reference vector and the vector to the mouse.
    float cross = refX * deltaY - refY * deltaX;

    angle = atan2(cross, dot) * (180 / M_PI);
    if (angle < 0) {
        angle += 360;
    }
}

void Gun::render(SDL_Renderer *renderer) {
    SDL_Rect dstRectInt = {static_cast<int>(dstRect.x), static_cast<int>(dstRect.y), static_cast<int>(dstRect.w), static_cast<int>(dstRect.h)};
    SDL_Point center = {dstRectInt.w / 2, dstRectInt.h / 2}; // The gun's texture is now centered

    if(angle >= 90 && angle <= 270){
        SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRectInt, angle, &center, SDL_FLIP_VERTICAL);
    } else{
        SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRectInt, angle, &center, SDL_FLIP_NONE);

    }
}

void Gun::fire(Renderer * renderer, SDL_FRect *vp) {
    float x = dstRect.x;
    float y = dstRect.y + dstRect.h / 2;
    float angela = angle * M_PI / 180;
    Bullet Bullet(x , y, 1000.0f, angela, renderer, vp);
    bullets.emplace_back(Bullet);
}

void Gun::updateBullets(float dt) {
    if(!bullets.empty()){
        for (auto& bullet : bullets) {
            bullet.update(dt);
        }
    }

    // TODO : Remove Bulltes off screen
}

void Gun::renderBullets(SDL_Renderer *renderer, SDL_FRect *vp) {

    if(bullets.size() > 200) {
        bullets.erase(bullets.begin(), bullets.begin() + 100);
    }

    if(!bullets.empty()){
        for (const auto& bullet : bullets) {
            bullet.render(renderer, vp);
        }
    }
}
