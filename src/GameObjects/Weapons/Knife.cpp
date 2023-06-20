#include "Knife.h"


Knife::Knife() {
    Renderer * render = RS::getInstance().get();
    int width, height;
    SDL_Surface * surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cout << "Failed to load image: " << IMG_GetError() << std::endl;
    }
    srcRect = {0, 0, surface->w, surface->h};
    texture = SDL_CreateTextureFromSurface(render, surface);
    if (!texture) {
        std::cout << "Failed to create texture: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    auto ratio = width/height;
    dstRect = { 500, 500, 64,static_cast<float>(48/ratio)};
}

void Knife::renderKnife(const SDL_FRect &viewport) {
    Renderer * render = RS::getInstance().get();
    SDL_FRect renderDstRect = dstRect;
    renderDstRect.x -= viewport.x;
    renderDstRect.y -= viewport.y;

    // Convert to SDL_Rect as SDL_RenderCopyEx requires this type
    SDL_Rect dstRectInt = {static_cast<int>(renderDstRect.x), static_cast<int>(renderDstRect.y),
                           static_cast<int>(renderDstRect.w), static_cast<int>(renderDstRect.h)};

    SDL_Point center = {dstRectInt.w / 2, dstRectInt.h / 2};

    // If the knife is pointing upwards or downwards, flip it vertically to maintain its orientation
    if(angle >= 90 && angle <= 270){
        SDL_RenderCopyEx(render, texture, nullptr, &dstRectInt, angle, &center, SDL_FLIP_VERTICAL);
    }
    else{
        SDL_RenderCopyEx(render, texture, nullptr, &dstRectInt, angle, &center, SDL_FLIP_NONE);
    }
}



void Knife::updateKnife(const SDL_FRect &enemyRect, float dt) {
    Player * player = PS::getInstance().get();
    float centerX = enemyRect.x + enemyRect.w / 2;
    float centerY = enemyRect.y + enemyRect.h / 2;

    dstRect.x = centerX - dstRect.w / 2;
    dstRect.y = centerY - dstRect.h / 2;

    float deltaX = player->dRect.x - centerX;
    float deltaY = player->dRect.y - centerY;

    float refX = 1.0f, refY = 0.0f;

    float dot = refX * deltaX + refY * deltaY;
    float cross = refX * deltaY - refY * deltaX;

    angle = atan2(cross, dot) * (180 / M_PI);
    if (angle < 0) {
        angle += 360;
    }
    float orbitRadius = enemyRect.w / 2;
    orbitRadius = 40;

    dstRect.x = centerX + orbitRadius * cos(angle * M_PI / 180.0f) - dstRect.w / 2;
    dstRect.y = centerY + orbitRadius * sin(angle * M_PI / 180.0f) - dstRect.h / 2;
}

void Knife::slash() {

}
